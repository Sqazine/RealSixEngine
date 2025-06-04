#pragma once
#include <string_view>
#include <vector>
#include <stack>
#include <fstream>
#include "Resource.h"
#include "RenderTask.h"
namespace RealSix
{
    class RenderTaskBuilder;
    class FrameGraph
    {
    public:
        FrameGraph() = default;
        ~FrameGraph() = default;

        FrameGraph(const FrameGraph &) = delete;
        FrameGraph &operator=(const FrameGraph &) = delete;

        FrameGraph(FrameGraph &&) = default;
        FrameGraph &operator=(FrameGraph &&) = default;

        template <typename Pass, typename... ArgTypes>
        RenderTask<Pass> *AddRenderTask(ArgTypes... args)
        {
            mRenderTasks.emplace_back(std::make_unique<RenderTask<Pass>>(args...));

            auto renderTask = mRenderTasks.back().get();

            RenderTaskBuilder builder(this, renderTask);
            renderTask->Setup(builder);

            MarkForCompile();

            return static_cast<RenderTask<Pass> *>(renderTask);
        }

        template <typename PassType>
        bool RemoveRenderTask()
        {
            if (mRenderTasks.empty())
                return false;
            auto iter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [](auto &renderTask)
                                     {
                                         std::string name = typeid(PassType).name();
                                         if (renderTask->GetName() == name)
                                             return true;
                                         return false;
                                     });

            if (iter != mRenderTasks.end())
            {
                mRenderTasks.erase(iter);
                MarkForCompile();
                return true;
            }
            return false;
        }

        template <typename PassType>
        bool HasOnly()
        {
            return mRenderTasks.size() == 1 && mRenderTasks[0]->GetName() == typeid(PassType).name();
        }

        template <typename PassType>
        bool Has()
        {
            auto iter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [](auto &renderTask)
                                     {
                                         std::string name = typeid(PassType).name();
                                         if (renderTask->GetName() == name)
                                             return true;
                                         return false;
                                     });

            if (iter != mRenderTasks.end())
                return true;
            return false;
        }

        template <typename DescriptionType, typename ActualType>
        Resource<DescriptionType, ActualType> *AddRetainedResource(std::string_view name, const DescriptionType &description, ActualType *actual = nullptr)
        {
            mResources[name] = std::make_unique<Resource<DescriptionType, ActualType>>(name, description, actual);
            return static_cast<Resource<DescriptionType, ActualType> *>(mResources[name].get());
        }

        template <typename DescriptionType, typename ActualType>
        std::vector<Resource<DescriptionType, ActualType> *> AddRetainedResource(std::string_view name, const DescriptionType &description, std::vector<ActualType *> &actuals = {})
        {
            std::vector<Resource<DescriptionType, ActualType> *> result;
            for (auto &actual : actuals)
            {
                mResources[name] = std::make_unique<Resource<DescriptionType, ActualType>>(name, description, actual);
                result.emplace_back(static_cast<Resource<DescriptionType, ActualType> *>(mResources[name]));
            }
            return result;
        }

        size_t GetTaskCount() const { return mRenderTasks.size(); }

        void Compile()
        {
            // Reference counting.
            for (auto &renderTask : mRenderTasks)
            {
                renderTask->mRefCount = renderTask->mResourceCreates.size() + renderTask->mResourceWrites.size();
            }

            for (auto &resource : mResources)
            {
                resource.second->mRefCount = resource.second->mReaders.size();
            }

            // Culling via flood fill from unreferenced resources.
            std::stack<ResourceBase *> unreferencedResources;

            for (auto &resource : mResources)
            {
                if (resource.second->mRefCount == 0 && resource.second->IsTransient())
                {
                    unreferencedResources.push(resource.second.get());
                }
            }

            while (!unreferencedResources.empty())
            {
                auto unreferencedResource = unreferencedResources.top();
                unreferencedResources.pop();

                auto creator = const_cast<RenderTaskBase *>(unreferencedResource->mCreator);

                if (creator->mRefCount > 0)
                {
                    creator->mRefCount--;
                }

                if (creator->mRefCount == 0 && !creator->IsPersistent())
                {
                    for (auto iteratee : creator->mResourceReads)
                    {
                        auto readResource = const_cast<ResourceBase *>(iteratee);
                        if (readResource->mRefCount > 0)
                        {
                            readResource->mRefCount--;
                        }

                        if (readResource->mRefCount == 0 && readResource->IsTransient())
                        {
                            unreferencedResources.push(readResource);
                        }
                    }
                }

                for (auto writer : unreferencedResource->mWriters)
                {
                    auto writeTask = const_cast<RenderTaskBase *>(writer);
                    if (writeTask->mRefCount > 0)
                    {
                        writeTask->mRefCount--;
                    }

                    if (writeTask->mRefCount == 0 && !writeTask->IsPersistent())
                    {
                        for (auto iteratee : writeTask->mResourceReads)
                        {
                            auto readResource = const_cast<ResourceBase *>(iteratee);
                            if (readResource->mRefCount > 0)
                            {
                                readResource->mRefCount--;
                            }

                            if (readResource->mRefCount == 0 && readResource->IsTransient())
                            {
                                unreferencedResources.push(readResource);
                            }
                        }
                    }
                }
            }

            // Timeline computation.
            mTimeline.clear();

            for (auto &renderTask : mRenderTasks)
            {
                if (renderTask->mRefCount == 0 && !renderTask->IsPersistent())
                {
                    continue;
                }

                std::vector<ResourceBase *> realizedResources, derealizedResources;

                for (auto resource : renderTask->mResourceCreates)
                {
                    realizedResources.push_back(const_cast<ResourceBase *>(resource));
                    if (resource->mReaders.empty() && resource->mWriters.empty())
                    {
                        derealizedResources.push_back(const_cast<ResourceBase *>(resource));
                    }
                }

                auto reads_writes = renderTask->mResourceReads;
                reads_writes.insert(reads_writes.end(), renderTask->mResourceWrites.begin(), renderTask->mResourceWrites.end());
                for (auto resource : reads_writes)
                {
                    if (!resource->IsTransient())
                    {
                        continue;
                    }

                    auto valid = false;
                    std::size_t lastIndex;

                    if (!resource->mReaders.empty())
                    {
                        auto lastReader = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [&resource](const std::unique_ptr<RenderTaskBase> &iteratee)
                                                       { return iteratee.get() == resource->mReaders.back(); });
                        if (lastReader != mRenderTasks.end())
                        {
                            valid = true;
                            lastIndex = std::distance(mRenderTasks.begin(), lastReader);
                        }
                    }

                    if (!resource->mWriters.empty())
                    {
                        auto lastWriter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [&resource](const std::unique_ptr<RenderTaskBase> &iteratee)
                                                       { return iteratee.get() == resource->mWriters.back(); });
                        if (lastWriter != mRenderTasks.end())
                        {
                            valid = true;
                            lastIndex = std::max(lastIndex, (size_t)std::distance(mRenderTasks.begin(), lastWriter));
                        }
                    }

                    if (valid && mRenderTasks[lastIndex] == renderTask)
                        derealizedResources.push_back(const_cast<ResourceBase *>(resource));
                }

                mTimeline.emplace_back(Step{renderTask.get(), realizedResources, derealizedResources});
            }
        }

        void Execute()
        {
            if (!mIsCompiled)
            {
                Compile();
                mIsCompiled = true;
            }

            for (auto &step : mTimeline)
            {
                for (auto resource : step.realizedResources)
                {
                    resource->Realize();
                }

                step.renderTask->Execute();

                for (auto resource : step.derealizedResources)
                {
                    resource->Derealize();
                }
            }
        }

        void Clear()
        {
            mRenderTasks.clear();
            mResources.clear();
        }

        void ExportGraphviz(std::string_view path)
        {
            std::ofstream stream(path.data());
            stream << "digraph framegraph \n{\n";

            stream << "rankdir = LR\n";
            stream << "bgcolor = black\n\n";
            stream << "node [shape=rectangle, fontname=\"helvetica\", fontsize=12]\n\n";

            for (auto &renderTask : mRenderTasks)
                stream << "\"" << renderTask->GetName() << "\" [label=\"" << renderTask->GetName() << "\\nRefs: " << renderTask->mRefCount << "\", style=filled, fillcolor=darkorange]\n";
            stream << "\n";

            for (auto &resource : mResources)
                stream << "\"" << resource.second->GetName() << "\" [label=\"" << resource.second->GetName() << "\\nRefs: " << resource.second->mRefCount << "\\nID: " << resource.second->GetId() << "\", style=filled, fillcolor= " << (resource.second->IsTransient() ? "skyblue" : "steelblue") << "]\n";
            stream << "\n";

            for (auto &renderTask : mRenderTasks)
            {
                stream << "\"" << renderTask->GetName() << "\" -> { ";
                for (auto &resource : renderTask->mResourceCreates)
                    stream << "\"" << resource->GetName() << "\" ";
                stream << "} [color=seagreen]\n";

                stream << "\"" << renderTask->GetName() << "\" -> { ";
                for (auto &resource : renderTask->mResourceWrites)
                    stream << "\"" << resource->GetName() << "\" ";
                stream << "} [color=gold]\n";
            }
            stream << "\n";

            for (auto &resource : mResources)
            {
                stream << "\"" << resource.second->GetName() << "\" -> { ";
                for (auto &renderTask : resource.second->mReaders)
                    stream << "\"" << renderTask->GetName() << "\" ";
                stream << "} [color=firebrick]\n";
            }
            stream << "}";
        }

    protected:
        friend class RenderTaskBuilder;

        void MarkForCompile()
        {
            mIsCompiled = false;
        }

        struct Step
        {
            RenderTaskBase *renderTask;
            std::vector<ResourceBase *> realizedResources;
            std::vector<ResourceBase *> derealizedResources;
        };

        bool mIsCompiled{false};
        std::vector<std::unique_ptr<RenderTaskBase>> mRenderTasks;
        std::unordered_map<std::string_view, std::unique_ptr<ResourceBase>> mResources;
        std::vector<Step> mTimeline;
    };

}