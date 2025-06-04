#pragma once
namespace RealSix
{
    class IGfxDevice;
    class GfxVulkanDevice;
    class GfxVulkanObject
    {
    public:
        GfxVulkanObject(IGfxDevice *device);
        virtual ~GfxVulkanObject() = default;

        GfxVulkanDevice *GetDevice() const;

    protected:
        GfxVulkanDevice *mDevice;
    };
} // namespace RealSix