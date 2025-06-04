#include "IGfxSwapChain.h"
#include "IGfxDevice.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanSwapChain.h"

namespace RealSix
{
    IGfxSwapChain::IGfxSwapChain(Window *window)
        : mWindow(window)
    {
    }

    IGfxSwapChain *IGfxSwapChain::Create(IGfxDevice *device, Window *window)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanSwapChain(device, window);
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