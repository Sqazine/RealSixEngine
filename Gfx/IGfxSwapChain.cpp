#include "IGfxSwapChain.hpp"
#include "IGfxDevice.hpp"
#include "Config/Config.hpp"
#include "Core/Logger.hpp"
#include "Gfx/VK/GfxVulkanSwapChain.hpp"

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