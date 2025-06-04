#include "GfxVulkanObject.hpp"
#include "GfxVulkanDevice.hpp"
#include "Gfx/IGfxDevice.hpp"
namespace RealSix
{
    GfxVulkanObject::GfxVulkanObject(IGfxDevice *device)
        : mDevice(static_cast<GfxVulkanDevice *>(device))
    {
    }

    GfxVulkanDevice *GfxVulkanObject::GetDevice() const
    {
        return mDevice;
    }

} // namespace RealSix