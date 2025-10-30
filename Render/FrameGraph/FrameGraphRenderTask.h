#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include "Core/Common.h"
#include "Core/NonCopyable.h"
namespace RealSix
{
    class FrameGraphRenderTaskBuilder;

    class FrameGraphRenderTaskBase : public NonCopyable
    {
    public:
        explicit FrameGraphRenderTaskBase(std::string_view name, bool persistent)
            : mName(name), mPersistent(persistent), mRefCount(0)
        {
        }

        virtual ~FrameGraphRenderTaskBase() override = default;

        std::string_view GetName() const { return mName; }
        void SetName(std::string_view name) { mName = name; }

        bool IsPersistent() const { return mPersistent; }
        void Setersistent(bool value) { mPersistent = value; }

    protected:
        virtual void Setup(FrameGraphRenderTaskBuilder &builder) = 0;
        virtual void Execute() = 0;

        friend class FrameGraph;
        friend class FrameGraphRenderTaskBuilder;

        std::string mName;
        bool mPersistent;
        std::vector<FrameGraphResourceBase *> mResourceCreates;
        std::vector<FrameGraphResourceBase *> mResourceReads;
        std::vector<FrameGraphResourceBase *> mResourceWrites;
        size_t mRefCount;
    };

    template <typename PassType>
    class FrameGraphRenderTask : public FrameGraphRenderTaskBase
    {
    public:
        explicit FrameGraphRenderTask(bool persistent,
                                      const std::function<void(PassType *, FrameGraphRenderTaskBuilder &)> &setupFunction,
                                      const std::function<void(PassType *)> &executeFunction)
            : FrameGraphRenderTaskBase(GetPrettyTypeName<PassType>(), persistent), mSetupFunction(setupFunction), mExecuteFunction(executeFunction)
        {
            mPass = std::make_unique<PassType>();
        }

        explicit FrameGraphRenderTask(bool persistent,
                                      const std::function<PassType *()> &createFunction,
                                      const std::function<void(PassType *, FrameGraphRenderTaskBuilder &)> &setupFunction,
                                      const std::function<void(PassType *)> &executeFunction)
            : FrameGraphRenderTaskBase(GetPrettyTypeName<PassType>(), persistent), mSetupFunction(setupFunction), mExecuteFunction(executeFunction)
        {
            mPass.reset(createFunction());
        }

        const PassType *GetPass() const { return mPass.get(); }

    protected:
        void Setup(FrameGraphRenderTaskBuilder &builder) override
        {
            mSetupFunction(mPass.get(), builder);
        }

        void Execute() override
        {
            mExecuteFunction(mPass.get());
        }

        std::unique_ptr<PassType> mPass;
        const std::function<void(PassType *, FrameGraphRenderTaskBuilder &)> mSetupFunction;
        const std::function<void(PassType *)> mExecuteFunction;
    };

}