#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxTexture.hpp"
#include "GfxVulkanObject.hpp"
#include "Gfx/IGfxDevice.hpp"
namespace RealSix
{
    class GfxVulkanTexture : public GfxVulkanObject, public IGfxTexture
    {
    public:
        GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImage swapchainImageRawHandle = VK_NULL_HANDLE);
        GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImageUsageFlags usage);
        ~GfxVulkanTexture() override;

        VkImageAspectFlagBits GetAspect();
        VkImage GetHandle() const { return mHandle; }
        VkImageView GetView() const { return mView; }
        VkSampler GetSampler() const { return mSampler; }

    private:
        void CreateImage(VkImageUsageFlags usage);
        void FillData();
        void CreateImageView();
        void CreateSampler();

        VkImage mHandle;
        VkDeviceMemory mMemory;
        VkImageView mView;

        VkSampler mSampler;
        bool mIsSwapChainImage;
    };
}