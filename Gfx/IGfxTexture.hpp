#pragma once
#include <cstdint>
#include "IGfxCommon.hpp"
#include "Math/Vector4.hpp"
namespace RealSix
{
    struct GfxTextureDesc
    {
        void *data{nullptr};

        uint32_t width{0};
        uint32_t height{0};

        bool useMipmap{false};
        uint32_t maxMipLevelCount{1};

        GfxFormat format{GfxFormat::R8G8B8A8_SRGB};
        uint8_t sampleCount{1};

        GfxFilter magFilter{GfxFilter::NEAREST};
        GfxFilter minFilter{GfxFilter::NEAREST};
        GfxAddressMode addressModeU{GfxAddressMode::REPEAT};
        GfxAddressMode addressModeV{GfxAddressMode::REPEAT};
        GfxAddressMode addressModeW{GfxAddressMode::REPEAT};
    };

    class IGfxDevice;
    class IGfxTexture
    {
    public:
        IGfxTexture(const GfxTextureDesc &desc);
        virtual ~IGfxTexture() = default;

        static IGfxTexture *Create(IGfxDevice *device, const GfxTextureDesc &desc);
        const GfxTextureDesc &GetDesc() const { return mDesc; }

        uint32_t GetMipLevelCount() const { return mDesc.useMipmap ? mDesc.maxMipLevelCount : 1; }

    protected:
        GfxTextureDesc mDesc;
    };

    struct GfxTextureAttachment
    {
        IGfxTexture *texture{nullptr};
        GfxAttachmentLoadOp loadOp{GfxAttachmentLoadOp::DONT_CARE};
        GfxAttachmentStoreOp storeOp{GfxAttachmentStoreOp::DONT_CARE};
    };
    struct GfxColorAttachment : public GfxTextureAttachment
    {
        Vector4f clearValue{Vector4f::ZERO};
        bool blendEnable{false};
        ColorChannelMask colorChannelMask = {ColorChannelMask::R | ColorChannelMask::G | ColorChannelMask::B | ColorChannelMask::A};
    };

    struct GfxDepthStencilAttachment : public GfxTextureAttachment
    {
        float depthClearValue{1.0f};
        uint32_t stencilClearValue{0};
    };
}