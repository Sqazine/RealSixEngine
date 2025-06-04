#include "GfxVulkanObject.h"
#include "GfxVulkanDevice.h"
#include "Gfx/IGfxDevice.h"
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