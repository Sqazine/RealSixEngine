#pragma once
#include <variant>
#include <memory>
#include "Core/Common.hpp"
namespace RealSix
{
    class FrameGraphRenderTaskBase;

    class FrameGraphResourceBase : public NonCopyable
    {
    public:
        explicit FrameGraphResourceBase(StringView name, const FrameGraphRenderTaskBase *creator)
            : mName(name), mCreator(creator), mRefCount(0)
        {
            static size_t sId = 0;
            mId = sId++;
        }
        virtual ~FrameGraphResourceBase() override = default;

        size_t GetId() const { return mId; }
        StringView GetName() const { return mName; }

        void SetName(StringView name) { mName = name; }

        bool IsTransient() const { return mCreator != nullptr; }

    protected:
        virtual void Realize() = 0;
        virtual void Derealize() = 0;

        friend class FrameGraph;
        friend class FrameGraphRenderTaskBuilder;

        size_t mId;
        String mName;
        const FrameGraphRenderTaskBase *mCreator;
        std::vector<const FrameGraphRenderTaskBase *> mReaders;
        std::vector<const FrameGraphRenderTaskBase *> mWriters;
        size_t mRefCount;
    };

    template <typename DescriptionType, typename ActualType>
    struct MissingRealizeImplementation : std::false_type
    {
    };

    template <typename DescriptionType, typename ActualType>
    std::unique_ptr<ActualType> Realize(const DescriptionType &description)
    {
        static_assert(MissingRealizeImplementation<DescriptionType, ActualType>::value, "Missing realize implementation for description - type pair.");
        return nullptr;
    }

    template <typename DescriptionType_, typename ActualType_>
    class FrameGraphResource : public FrameGraphResourceBase
    {
    public:
        using DescriptionType = DescriptionType_;
        using ActualType = ActualType_;

        explicit FrameGraphResource(StringView name, const FrameGraphRenderTaskBase *creator, const DescriptionType &description)
            : FrameGraphResourceBase(name, creator), mDescription(description), mActualData(std::make_unique<ActualType>())
        {
            // Transient (normal) constructor.
        }

        explicit FrameGraphResource(StringView name, const DescriptionType &description, ActualType *actualData = nullptr)
            : FrameGraphResourceBase(name, nullptr), mDescription(description), mActualData(actualData)
        {
            // Retained (import) constructor.
            if (!actualData)
                mActualData = RealSix::Realize<DescriptionType, ActualType>(mDescription);
        }

        ~FrameGraphResource() override = default;

        const DescriptionType &GetDescription() const { return mDescription; }

        ActualType *GetActualData() const
        {
            return std::holds_alternative<std::unique_ptr<ActualType>>(mActualData) ? std::get<std::unique_ptr<ActualType>>(mActualData).get() : std::get<ActualType *>(mActualData);
        }

    protected:
        void Realize() override
        {
            if (IsTransient())
                std::get<std::unique_ptr<ActualType>>(mActualData) = RealSix::Realize<DescriptionType, ActualType>(mDescription);
        }
        void Derealize() override
        {
            if (IsTransient())
                std::get<std::unique_ptr<ActualType>>(mActualData).reset();
        }

        DescriptionType mDescription;
        std::variant<std::unique_ptr<ActualType>, ActualType *> mActualData;
    };
}