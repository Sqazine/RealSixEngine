#pragma once
#include "IGfxDevice.hpp"
#include "IGfxCommon.hpp"
#include <vector>
namespace RealSix
{
    struct GfxBufferDesc
    {
        size_t elementSize;
        size_t bufferSize;
        void *data{nullptr};

        bool IsArrayBuffer()
        {
            return bufferSize > elementSize;
        }
    };
    class IGfxBuffer
    {
    public:
        IGfxBuffer() = default;
        virtual ~IGfxBuffer() = default;

        virtual size_t GetAddress() const = 0;
    };

    class GfxVertexBuffer
    {
    public:
        GfxVertexBuffer() = default;
        ~GfxVertexBuffer() { mGfxBuffer.reset(nullptr); }

        static GfxVertexBuffer *Create(IGfxDevice *device, const GfxBufferDesc &desc);

        IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    class GfxIndexBuffer
    {
    public:
        GfxIndexBuffer() = default;
        ~GfxIndexBuffer() { mGfxBuffer.reset(nullptr); }

        static GfxIndexBuffer *Create(IGfxDevice *device, const GfxBufferDesc &desc);

        IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }
        GfxIndexType GetIndexType() const { return mIndexType; }
        size_t GetElementCount() const { return mElementCount; }

    protected:
        size_t mElementCount;
        GfxIndexType mIndexType{GfxIndexType::UINT32};

        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    class GfxUniformBuffer
    {
    public:
        GfxUniformBuffer() = default;
        ~GfxUniformBuffer() { mGfxBuffer.reset(nullptr); }

        static GfxUniformBuffer *Create(IGfxDevice *device, const GfxBufferDesc &desc);

        void SetData(const GfxBufferDesc &desc);

        const IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    class GfxShaderStorageBuffer
    {
    public:
        GfxShaderStorageBuffer() = default;
        ~GfxShaderStorageBuffer() { mGfxBuffer.reset(nullptr); }

        static GfxShaderStorageBuffer *Create(IGfxDevice *device, const GfxBufferDesc &desc);

        IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };
}