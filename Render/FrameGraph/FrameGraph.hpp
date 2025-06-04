#pragma once

#include <vector>
#include <stack>
#include <fstream>
#include "FrameGraphResource.hpp"
#include "FrameGraphRenderTask.hpp"
#include "Core/Common.hpp"
#include "Core/Logger.hpp"

namespace RealSix
{
    class FrameGraphRenderTaskBuilder;
    class FrameGraph : public NonCopyable
    {
    public:
        FrameGraph() = default;
        ~FrameGraph() override = default;

        template <typename Pass, typename... ArgTypes>
        FrameGraphRenderTask<Pass> *AddRenderTask(ArgTypes... args)
        {
            mRenderTasks.emplace_back(std::make_unique<FrameGraphRenderTask<Pass>>(args...));

            auto renderTask = mRenderTasks.back().get();

            FrameGraphRenderTaskBuilder builder(this, renderTask);
            renderTask->Setup(builder);

            MarkForCompile();

            return static_cast<FrameGraphRenderTask<Pass> *>(renderTask);
        }

        template <typename Pass, typename... ArgTypes>
        FrameGraphRenderTask<Pass> *InsertRenderTask(size_t slot, ArgTypes... args)
        {
            if (slot >= mRenderTasks.size())
            {
                REALSIX_LOG_WARN("InsertRenderTask slot {} is out of range, use AddRenderTask instead.", slot);
                return AddRenderTask<Pass>(args...);
            }
            auto newTask = std::make_unique<FrameGraphRenderTask<Pass>>(args...);
            mRenderTasks.insert(mRenderTasks.begin() + slot, std::move(newTask));

            auto renderTask = mRenderTasks[slot].get();

            FrameGraphRenderTaskBuilder builder(this, renderTask);
            renderTask->Setup(builder);

            MarkForCompile();

            return static_cast<FrameGraphRenderTask<Pass> *>(renderTask);
        }

        template <typename PassType>
        bool RemoveRenderTask()
        {
            if (mRenderTasks.empty())
                return false;
            auto iter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [](auto &renderTask)
                                     {
                                         String name = GetPrettyTypeName<PassType>();
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
            return mRenderTasks.size() == 1 && mRenderTasks[0]->GetName() == GetPrettyTypeName<PassType>();
        }

        template <typename PassType>
        int32_t GetSlotOf()
        {
            auto iter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [](auto &renderTask)
                                     {
                                         String name = GetPrettyTypeName<PassType>();
                                         if (renderTask->GetName() == name)
                                             return true;
                                         return false;
                                     });

            if (iter != mRenderTasks.end())
                return std::distance(mRenderTasks.begin(), iter);
            return static_cast<size_t>(-1);
        }

        template <typename PassType>
        bool Has()
        {
            auto iter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [](auto &renderTask)
                                     {
                                         String name = GetPrettyTypeName<PassType>();
                                         if (renderTask->GetName() == name)
                                             return true;
                                         return false;
                                     });

            if (iter != mRenderTasks.end())
                return true;
            return false;
        }

        template <typename DescriptionType, typename ActualType>
        FrameGraphResource<DescriptionType, ActualType> *AddRetainedResource(StringView name, const DescriptionType &description, ActualType *actual = nullptr)
        {
            mResources[name] = std::make_unique<FrameGraphResource<DescriptionType, ActualType>>(name, description, actual);
            return static_cast<FrameGraphResource<DescriptionType, ActualType> *>(mResources[name].get());
        }

        template <typename DescriptionType, typename ActualType>
        std::vector<FrameGraphResource<DescriptionType, ActualType> *> AddRetainedResource(StringView name, const DescriptionType &description, std::vector<ActualType *> &actuals = {})
        {
            std::vector<FrameGraphResource<DescriptionType, ActualType> *> result;
            for (auto &actual : actuals)
            {
                mResources[name] = std::make_unique<FrameGraphResource<DescriptionType, ActualType>>(name, description, actual);
                result.emplace_back(static_cast<FrameGraphResource<DescriptionType, ActualType> *>(mResources[name]));
            }
            return result;
        }

        size_t GetTaskCount() const { return mRenderTasks.size(); }

        bool IsEmpty() const { return mRenderTasks.empty(); }

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
            std::stack<FrameGraphResourceBase *> unreferencedResources;

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

                auto creator = const_cast<FrameGraphRenderTaskBase *>(unreferencedResource->mCreator);

                if (creator->mRefCount > 0)
                {
                    creator->mRefCount--;
                }

                if (creator->mRefCount == 0 && !creator->IsPersistent())
                {
                    for (auto iteratee : creator->mResourceReads)
                    {
                        auto readResource = const_cast<FrameGraphResourceBase *>(iteratee);
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
                    auto writeTask = const_cast<FrameGraphRenderTaskBase *>(writer);
                    if (writeTask->mRefCount > 0)
                    {
                        writeTask->mRefCount--;
                    }

                    if (writeTask->mRefCount == 0 && !writeTask->IsPersistent())
                    {
                        for (auto iteratee : writeTask->mResourceReads)
                        {
                            auto readResource = const_cast<FrameGraphResourceBase *>(iteratee);
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

                std::vector<FrameGraphResourceBase *> realizedResources, derealizedResources;

                for (auto resource : renderTask->mResourceCreates)
                {
                    realizedResources.push_back(const_cast<FrameGraphResourceBase *>(resource));
                    if (resource->mReaders.empty() && resource->mWriters.empty())
                    {
                        derealizedResources.push_back(const_cast<FrameGraphResourceBase *>(resource));
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
                        auto lastReader = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [&resource](const std::unique_ptr<FrameGraphRenderTaskBase> &iteratee)
                                                       { return iteratee.get() == resource->mReaders.back(); });
                        if (lastReader != mRenderTasks.end())
                        {
                            valid = true;
                            lastIndex = std::distance(mRenderTasks.begin(), lastReader);
                        }
                    }

                    if (!resource->mWriters.empty())
                    {
                        auto lastWriter = std::find_if(mRenderTasks.begin(), mRenderTasks.end(), [&resource](const std::unique_ptr<FrameGraphRenderTaskBase> &iteratee)
                                                       { return iteratee.get() == resource->mWriters.back(); });
                        if (lastWriter != mRenderTasks.end())
                        {
                            valid = true;
                            lastIndex = std::max(lastIndex, (size_t)std::distance(mRenderTasks.begin(), lastWriter));
                        }
                    }

                    if (valid && mRenderTasks[lastIndex] == renderTask)
                        derealizedResources.push_back(const_cast<FrameGraphResourceBase *>(resource));
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

        void ExportGraphviz(StringView path)
        {
            std::ofstream stream(path.CString());
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
        friend class FrameGraphRenderTaskBuilder;

        void MarkForCompile()
        {
            mIsCompiled = false;
        }

        struct Step
        {
            FrameGraphRenderTaskBase *renderTask;
            std::vector<FrameGraphResourceBase *> realizedResources;
            std::vector<FrameGraphResourceBase *> derealizedResources;
        };

        bool mIsCompiled{false};
        std::vector<std::unique_ptr<FrameGraphRenderTaskBase>> mRenderTasks;
        std::unordered_map<StringView, std::unique_ptr<FrameGraphResourceBase>> mResources;
        std::vector<Step> mTimeline;
    };

}