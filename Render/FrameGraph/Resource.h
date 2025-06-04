#pragma once
#include <variant>
#include <memory>
namespace RealSix
{
    class RenderTaskBase;

    class ResourceBase
    {
    public:
        explicit ResourceBase(std::string_view name, const RenderTaskBase *creator)
            : mName(name), mCreator(creator), mRefCount(0)
        {
            static size_t sId = 0;
            mId = sId++;
        }
        virtual ~ResourceBase() = default;

        ResourceBase(const ResourceBase &) = delete;
        ResourceBase &operator=(const ResourceBase &) = delete;

        ResourceBase(ResourceBase &&) = default;
        ResourceBase &operator=(ResourceBase &&) = default;

        size_t GetId() const { return mId; }
        std::string_view GetName() const { return mName; }

        void SetName(std::string_view name) { mName = name; }

        bool IsTransient() const { return mCreator != nullptr; }

    protected:
        virtual void Realize() = 0;
        virtual void Derealize() = 0;

        friend class FrameGraph;
        friend class RenderTaskBuilder;

        size_t mId;
        std::string mName;
        const RenderTaskBase *mCreator;
        std::vector<const RenderTaskBase *> mReaders;
        std::vector<const RenderTaskBase *> mWriters;
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
    class Resource : public ResourceBase
    {
    public:
        using DescriptionType = DescriptionType_;
        using ActualType = ActualType_;

        explicit Resource(std::string_view name, const RenderTaskBase *creator, const DescriptionType &description)
            : ResourceBase(name, creator), mDescription(description), mActualData(std::make_unique<ActualType>())
        {
            // Transient (normal) constructor.
        }

        explicit Resource(std::string_view name, const DescriptionType &description, ActualType *actualData = nullptr)
            : ResourceBase(name, nullptr), mDescription(description), mActualData(actualData)
        {
            // Retained (import) constructor.
            if (!actualData)
                mActualData = RealSix::Realize<DescriptionType, ActualType>(mDescription);
        }

        ~Resource()
        {
        }

        Resource(const Resource &) = delete;
        Resource &operator=(const Resource &) = delete;

        Resource(Resource &&) = default;
        Resource &operator=(Resource &&) = default;

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