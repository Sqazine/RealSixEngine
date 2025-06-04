#pragma once

namespace RealSix
{
    class FrameGraph;
    class FrameGraphRenderTaskBase;

    class FrameGraphRenderTaskBuilder : public NonCopyable
    {
    public:
        explicit FrameGraphRenderTaskBuilder(FrameGraph *frameGraph, FrameGraphRenderTaskBase *task)
            : mFrameGraph(frameGraph), mTask(task)
        {
        }

        virtual ~FrameGraphRenderTaskBuilder() override = default;

        template <typename ResourceType, typename DescriptionType>
        ResourceType *CreateResource(StringView name, const DescriptionType &description)
        {
            static_assert(std::is_same<typename ResourceType::DescriptionType, DescriptionType>::value, "Description does not match the resource.");
            mFrameGraph->mResources[name] = std::make_unique<ResourceType>(name, mTask, description);

            const auto resource = mFrameGraph->mResources[name].get();
            mTask->mResourceCreates.push_back(resource);
            return static_cast<ResourceType *>(resource);
        }

        template <typename ResourceType>
        ResourceType *Read(ResourceType *resource)
        {
            resource->mReaders.push_back(mTask);
            mTask->mResourceReads.push_back(resource);
            return resource;
        }

        template <typename ResourceType>
        ResourceType *Write(ResourceType *resource)
        {
            resource->mWriters.push_back(mTask);
            mTask->mResourceWrites.push_back(resource);
            return resource;
        }

        template <typename ResourceType>
        std::vector<ResourceType *> Write(std::vector<ResourceType *> &resources)
        {
            for (auto &resource : resources)
            {
                resource->mWriters.push_back(mTask);
                mTask->mResourceWrites.push_back(resource);
            }
            return resources;
        }

    protected:
        FrameGraph *mFrameGraph;
        FrameGraphRenderTaskBase *mTask;
    };
}