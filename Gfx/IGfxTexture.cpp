#include "IGfxTexture.h"
#include "IGfxDevice.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "Gfx/VK/GfxVulkanTexture.h"
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
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
        return nullptr;
    }
}