#include "IGfxTexture.hpp"
#include "IGfxDevice.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "Core/Marco.hpp"
#include "Gfx/VK/GfxVulkanTexture.hpp"
namespace RealSix
{
    IGfxTexture::IGfxTexture(const GfxTextureDesc &desc)
        : mDesc(desc)
    {
    }

    IGfxTexture *IGfxTexture::Create(IGfxDevice *device, const GfxTextureDesc &desc)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanTexture(device, desc);
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr;
    }
}