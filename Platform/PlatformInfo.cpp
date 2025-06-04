#include "PlatformInfo.hpp"
#include "SDL3/SDL3PlatformInfo.hpp"
namespace RealSix
{
    HardwareInfo *HardwareInfo::Create()
    {
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
        auto sdl3HardwareInfo = new SDL3HardwareInfo();
        sdl3HardwareInfo->ObtainDisplayInfo();
        return sdl3HardwareInfo;
#else
#error "Not Support Platform, only windows is available now!"
#endif
    }

    VulkanPlatformInfo *VulkanPlatformInfo::Create()
    {
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
       return new SDL3VulkanPlatformInfo();
#else
#error "Not Support Platform, only windows is available now!"
#endif
    }
}