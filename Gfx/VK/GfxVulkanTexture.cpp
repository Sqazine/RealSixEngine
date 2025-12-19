#include "GfxVulkanTexture.hpp"
#include "GfxVulkanDevice.hpp"
#include "GfxVulkanAllocator.hpp"
namespace RealSix
{
    GfxVulkanTexture::GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImage swapchainImageRawHandle)
        : IGfxTexture(desc), GfxVulkanObject(device)
    {
        if (swapchainImageRawHandle == VK_NULL_HANDLE)
        {
            mIsSwapChainImage = false;
            CreateImage(VkImageUsageFlagBits(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
            
            if (mDesc.data)
                FillData();
        }
        else
        {
            mIsSwapChainImage = true;
            mHandle = swapchainImageRawHandle;
        }
        CreateImageView();
        CreateSampler();
    }

    GfxVulkanTexture::GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImageUsageFlags usage)
        : IGfxTexture(desc), GfxVulkanObject(device), mIsSwapChainImage(false)
    {
        CreateImage(usage);
        CreateImageView();
        CreateSampler();
    }

    GfxVulkanTexture::~GfxVulkanTexture()
    {
        mDevice->WaitIdle();
        VkDevice device = mDevice->GetLogicDevice();
        vkDestroyImageView(device, mView, nullptr);
        vkDestroySampler(device, mSampler, nullptr);
        if (!mIsSwapChainImage)
        {
            vkDestroyImage(device, mHandle, nullptr);
            vkFreeMemory(device, mMemory, nullptr);
        }
    }

    VkImageAspectFlagBits GfxVulkanTexture::GetAspect()
    {
        return GetAspectFromFormat(mDesc.format);
    }

    void GfxVulkanTexture::CreateImage(VkImageUsageFlags usage)
    {
        VkImageCreateInfo imageInfo{};
        ZeroVulkanStruct(imageInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = mDesc.width;
        imageInfo.extent.height = mDesc.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mDesc.useMipmap ? mDesc.maxMipLevelCount : 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = ToVkFormat(mDesc.format);
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = (VkSampleCountFlagBits)mDesc.sampleCount;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK(vkCreateImage(mDevice->GetLogicDevice(), &imageInfo, nullptr, &mHandle));

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(mDevice->GetLogicDevice(), mHandle, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VK_CHECK(vkAllocateMemory(mDevice->GetLogicDevice(), &allocInfo, nullptr, &mMemory));

        vkBindImageMemory(mDevice->GetLogicDevice(), mHandle, mMemory, 0);
    }

    void GfxVulkanTexture::FillData()
    {
        float imageSize = mDesc.width * mDesc.height * GetFormatByteCount(mDesc.format);
        std::unique_ptr<GfxVulkanBuffer> stagingBuffer;
        stagingBuffer.reset(GfxVulkanAllocator::CreateStagingBuffer(mDevice, imageSize));

        GfxVulkanAllocator::SetBufferData(stagingBuffer.get(), (size_t)imageSize, mDesc.data);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(mDevice, GfxCommandType::TRANSFER, true);
        commandBuffer->Begin();
        commandBuffer->TransitionImageLayout(this, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        commandBuffer->CopyBufferToImage(stagingBuffer.get(), this, mDesc.width, mDesc.height);
        commandBuffer->TransitionImageLayout(this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        commandBuffer->End();

        commandBuffer->Submit();

        stagingBuffer.reset();
    }

    void GfxVulkanTexture::CreateImageView()
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = mHandle;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = ToVkFormat(mDesc.format);
        viewInfo.subresourceRange.aspectMask = GetAspectFromFormat(mDesc.format);
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mDesc.useMipmap ? mDesc.maxMipLevelCount : 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(mDevice->GetLogicDevice(), &viewInfo, nullptr, &mView));
    }

    void GfxVulkanTexture::CreateSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = ToVkFilter(mDesc.magFilter);
        samplerInfo.minFilter = ToVkFilter(mDesc.minFilter);
        samplerInfo.addressModeU = ToVkSamplerAddressMode(mDesc.addressModeU);
        samplerInfo.addressModeV = ToVkSamplerAddressMode(mDesc.addressModeV);
        samplerInfo.addressModeW = ToVkSamplerAddressMode(mDesc.addressModeW);
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = mDevice->GetMaxSamplerAnisotropy();
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f;

        VK_CHECK(vkCreateSampler(mDevice->GetLogicDevice(), &samplerInfo, nullptr, &mSampler));
    }

}