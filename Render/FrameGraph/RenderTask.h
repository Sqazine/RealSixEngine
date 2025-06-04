#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
namespace RealSix
{
    class RenderTaskBuilder;

    class RenderTaskBase
    {
    public:
        explicit RenderTaskBase(std::string_view name, bool persistent)
            : mName(name), mPersistent(persistent), mRefCount(0)
        {
        }

        virtual ~RenderTaskBase() = default;

        RenderTaskBase(const RenderTaskBase &) = delete;
        RenderTaskBase &operator=(const RenderTaskBase &) = delete;

        RenderTaskBase(RenderTaskBase &&) = default;
        RenderTaskBase &operator=(RenderTaskBase &&) = default;

        std::string_view GetName() const { return mName; }
        void SetName(std::string_view name) { mName = name; }

        bool IsPersistent() const { return mPersistent; }
        void Setersistent(bool value) { mPersistent = value; }

    protected:
        virtual void Setup(RenderTaskBuilder &builder) = 0;
        virtual void Execute() = 0;

        friend class FrameGraph;
        friend class RenderTaskBuilder;

        std::string mName;
        bool mPersistent;
        std::vector<ResourceBase *> mResourceCreates;
        std::vector<ResourceBase *> mResourceReads;
        std::vector<ResourceBase *> mResourceWrites;
        size_t mRefCount;
    };

    template <typename PassType>
    class RenderTask : public RenderTaskBase
    {
    public:
        explicit RenderTask(bool persistent,
                            const std::function<void(PassType *, RenderTaskBuilder &)> &setupFunction,
                            const std::function<void(PassType *)> &executeFunction)
            : RenderTaskBase(typeid(PassType).name(), persistent), mSetupFunction(setupFunction), mExecuteFunction(executeFunction)
        {
            mPass = std::make_unique<PassType>();
        }

        explicit RenderTask(bool persistent,
                            const std::function<PassType *()> &createFunction,
                            const std::function<void(PassType *, RenderTaskBuilder &)> &setupFunction,
                            const std::function<void(PassType *)> &executeFunction)
            : RenderTaskBase(typeid(PassType).name(), persistent), mSetupFunction(setupFunction), mExecuteFunction(executeFunction)
        {
            mPass.reset(createFunction());
        }

        RenderTask(const RenderTask &) = delete;
        RenderTask &operator=(const RenderTask &) = delete;

        RenderTask(RenderTask &&) = default;
        RenderTask &operator=(RenderTask &&) = default;

        const PassType *GetPass() const { return mPass.get(); }

    protected:
        void Setup(RenderTaskBuilder &builder) override
        {
            mSetupFunction(mPass.get(), builder);
        }

        void Execute() override
        {
            mExecuteFunction(mPass.get());
        }

        std::unique_ptr<PassType> mPass;
        const std::function<void(PassType *, RenderTaskBuilder &)> mSetupFunction;
        const std::function<void(PassType *)> mExecuteFunction;
    };

}