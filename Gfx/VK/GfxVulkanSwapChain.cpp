#include "GfxVulkanSwapChain.hpp"
#include "GfxVulkanDevice.hpp"
#include "Platform/PlatformInfo.hpp"
#include "Core/Config.hpp"
#include "Math/Math.hpp"
#include "Core/Marco.hpp"
namespace RealSix
{
    GfxVulkanSwapChain::GfxVulkanSwapChain(IGfxDevice *device, Window *window)
        : GfxVulkanObject(device), IGfxSwapChain(window), mHandle(VK_NULL_HANDLE), mNextFrameIndex(0)
    {
        CreateSurface();
        ObtainPresentQueue();
        CreateSwapChain();
        CreateBackTextures();
        CreateCommandBuffers();
        CreateSyncObjects();
        SetupAttachments();
    }

    GfxVulkanSwapChain::~GfxVulkanSwapChain()
    {
        CleanUpResource();
        vkDestroySurfaceKHR(mDevice->GetInstance(), mSurface, nullptr);
    }

    const VkSurfaceFormatKHR GfxVulkanSwapChain::GetSurfaceFormat() const
    {
        return mSurfaceFormat;
    }

    void GfxVulkanSwapChain::BeginFrame()
    {
        GfxVulkanCommandBuffer *cmdBuffer =static_cast<GfxVulkanCommandBuffer*>(GetCurrentBackCommandBuffer());

        auto curFence = cmdBuffer->GetFence();
        curFence->Wait(true, UINT64_MAX);

        auto result = vkAcquireNextImageKHR(mDevice->GetLogicDevice(), mHandle, UINT64_MAX, mPresentSemaphore[mCurrentFrameIndex]->GetHandle(), VK_NULL_HANDLE, &mNextFrameIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            REALSIX_LOG_INFO("Swap chain out of date, resizing...");
            Resize(mWindow->GetSize());
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            REALSIX_LOG_ERROR("Failed to acquire next image!");
            return;
        }

        curFence->Reset();

        cmdBuffer->Begin();
        cmdBuffer->TransitionImageLayout(GetCurrentSwapChainBackTexture(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)GetExtent().width;
        viewport.height = (float)GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmdBuffer->GetHandle(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = GetExtent();
        vkCmdSetScissor(cmdBuffer->GetHandle(), 0, 1, &scissor);

        SetupAttachments();
    }

    void GfxVulkanSwapChain::EndFrame()
    {
        GfxVulkanCommandBuffer *cmdBuffer = static_cast<GfxVulkanCommandBuffer*>(GetCurrentBackCommandBuffer());

        cmdBuffer->TransitionImageLayout(GetCurrentSwapChainBackTexture(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        cmdBuffer->End();

        cmdBuffer->Submit(mPresentSemaphore[mCurrentFrameIndex].get());
        GfxVulkanSemaphore *renderFinishedSemaphore = cmdBuffer->GetSignalSemaphore();

        Present(renderFinishedSemaphore);

        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameOverlapCount;
    }

    uint8_t GfxVulkanSwapChain::GetBackBufferCount() const
    {
        return mSwapChainImageCount;
    }

    uint8_t GfxVulkanSwapChain::GetCurrentBackBufferIndex() const
    {
        return mCurrentFrameIndex;
    }

    IGfxCommandBuffer *GfxVulkanSwapChain::GetCurrentBackCommandBuffer() const
    {
        return mGfxCommandBuffer[mCurrentFrameIndex].get();
    }

    VkExtent2D GfxVulkanSwapChain::GetExtent() const
    {
        return mExtent;
    }

    uint32_t GfxVulkanSwapChain::GetNextFrameIndex() const
    {
        return mNextFrameIndex;
    }

    GfxVulkanTexture *GfxVulkanSwapChain::GetCurrentSwapChainBackTexture() const
    {
        return mSwapChainColorBackTextures[GetNextFrameIndex()];
    }

    void GfxVulkanSwapChain::Present(const GfxVulkanSemaphore *waitFor)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &mHandle;
        presentInfo.pImageIndices = &mNextFrameIndex;
        if (waitFor)
        {
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &waitFor->GetHandle();
        }

        auto result = vkQueuePresentKHR(mPresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            REALSIX_LOG_INFO("Swap chain out of date, resizing...");
            Resize(mWindow->GetSize());
        }
        else if (result != VK_SUCCESS)
        {
            REALSIX_LOG_ERROR("Failed to present swap chain!");
        }
    }

    void GfxVulkanSwapChain::CreateSwapChain()
    {
        SwapChainDetails swapChainDetail = QuerySwapChainDetails();
        mSurfaceFormat = ChooseSwapChainSurfaceFormat(swapChainDetail.surfaceFormats);
        mPresentMode = ChooseSwapChainPresentMode(swapChainDetail.presentModes);
        mExtent = ChooseSwapChainExtent(swapChainDetail.surfaceCapabilities);

        uint32_t imageCount = static_cast<uint32_t>(GfxConfig::GetInstance().GetBackBufferCount());
        imageCount = Math::Clamp(imageCount, swapChainDetail.surfaceCapabilities.minImageCount, swapChainDetail.surfaceCapabilities.maxImageCount);

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.surface = mSurface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = mSurfaceFormat.format;
        createInfo.imageColorSpace = mSurfaceFormat.colorSpace;
        createInfo.imageExtent = mExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = mDevice->GetPhysicalDeviceSpec().queueFamilyIndices;
        if (indices.graphicsFamilyIdx.value() != mPresentFamilyIdx)
        {
            uint32_t queueFamilyIndices[] = {indices.graphicsFamilyIdx.value(), mPresentFamilyIdx};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainDetail.surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = mPresentMode;
        createInfo.oldSwapchain = (mHandle == VK_NULL_HANDLE) ? mHandle : VK_NULL_HANDLE;

        VK_CHECK(vkCreateSwapchainKHR(mDevice->GetLogicDevice(), &createInfo, nullptr, &mHandle));

        vkGetSwapchainImagesKHR(mDevice->GetLogicDevice(), mHandle, &mSwapChainImageCount, nullptr);

        mFrameOverlapCount = mSwapChainImageCount - 1;
    }

    void GfxVulkanSwapChain::CreateBackTextures()
    {
        std::vector<VkImage> images(mSwapChainImageCount);
        vkGetSwapchainImagesKHR(mDevice->GetLogicDevice(), mHandle, &mSwapChainImageCount, images.data());

        GfxTextureDesc desc;
        desc.width = mExtent.width;
        desc.height = mExtent.height;
        desc.format = ToFormat(mSurfaceFormat.format);

        mSwapChainColorBackTextures.resize(mSwapChainImageCount);
        for (uint32_t i = 0; i < mSwapChainImageCount; i++)
        {
            mSwapChainColorBackTextures[i] = new GfxVulkanTexture(mDevice, desc, images[i]);
        }

        const bool useMsaa = GfxConfig::GetInstance().GetMsaa() > Msaa::X1;
        if (useMsaa)
        {
            desc.sampleCount = static_cast<uint8_t>(GfxConfig::GetInstance().GetMsaa());
            mColorBackTexture = std::make_unique<GfxVulkanTexture>(mDevice, desc, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        }

        desc.format = ToFormat(mDevice->FindDepthFormat());
        mDepthBackTexture = std::make_unique<GfxVulkanTexture>(mDevice, desc, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    void GfxVulkanSwapChain::CreateSurface()
    {
        VulkanPlatformInfo *platformInfo = PlatformInfo::GetInstance().GetVulkanPlatformInfo();
        mSurface = platformInfo->CreateSurface(mWindow, mDevice->GetInstance());
    }

    void GfxVulkanSwapChain::CreateCommandBuffers()
    {
        mGfxCommandBuffer.resize(mFrameOverlapCount);
        for (auto &cmdBuffer : mGfxCommandBuffer)
            cmdBuffer = std::make_unique<GfxVulkanCommandBuffer>(mDevice, GfxCommandType::GRAPHICS, false);
    }

    void GfxVulkanSwapChain::CreateSyncObjects()
    {
        mPresentSemaphore.resize(mFrameOverlapCount);
        for (auto &semaphore : mPresentSemaphore)
            semaphore = std::make_unique<GfxVulkanSemaphore>(mDevice);
    }

    void GfxVulkanSwapChain::SetupAttachments()
    {
        const bool useMsaa = GfxConfig::GetInstance().GetMsaa() > Msaa::X1;

        mColorAttachment.texture = useMsaa ? mColorBackTexture.get() : GetCurrentSwapChainBackTexture();
        mDepthAttachment.texture = mDepthBackTexture.get();
    }

    VkSurfaceFormatKHR GfxVulkanSwapChain::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const auto &availableFormat : availableFormats)
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        return availableFormats[0];
    }

    VkPresentModeKHR GfxVulkanSwapChain::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        for (const auto &availablePresentMode : availablePresentModes)
            if (gfxConfig.GetVSync() && availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
                return availablePresentMode;

        for (const auto &availablePresentMode : availablePresentModes)
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;

        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

    VkExtent2D GfxVulkanSwapChain::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() && capabilities.currentExtent.height != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;
        else
        {
            auto size = mWindow->GetSize();
            VkExtent2D actualExtent =
                {
                    (uint32_t)size.x,
                    (uint32_t)size.y,
                };

            actualExtent.width = Math::Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = Math::Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    SwapChainDetails GfxVulkanSwapChain::QuerySwapChainDetails()
    {
        SwapChainDetails result;

        auto physicalDevice = mDevice->GetPhysicalDeviceSpec().handle;

        uint32_t count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &count, nullptr);
        result.surfaceFormats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &count, result.surfaceFormats.data());

        count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &count, nullptr);
        result.presentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &count, result.presentModes.data());

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, mSurface, &result.surfaceCapabilities);

        return result;
    }

    void GfxVulkanSwapChain::ObtainPresentQueue()
    {
        auto physicalDeviceSpec = mDevice->GetPhysicalDeviceSpec();

        auto physicalDevice = physicalDeviceSpec.handle;

        auto graphicsFamilyIdx = physicalDeviceSpec.queueFamilyIndices.graphicsFamilyIdx;
        auto computeFamilyIdx = physicalDeviceSpec.queueFamilyIndices.computeFamilyIdx;
        auto transferFamilyIdx = physicalDeviceSpec.queueFamilyIndices.transferFamilyIdx;

        VkBool32 presentSupport = false;
        if (graphicsFamilyIdx.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, graphicsFamilyIdx.value(), mSurface, &presentSupport);

            if (presentSupport)
                mPresentFamilyIdx = graphicsFamilyIdx.value();
        }
        else if (computeFamilyIdx.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, computeFamilyIdx.value(), mSurface, &presentSupport);

            if (presentSupport)
                mPresentFamilyIdx = computeFamilyIdx.value();
        }
        else if (transferFamilyIdx.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, transferFamilyIdx.value(), mSurface, &presentSupport);

            if (presentSupport)
                mPresentFamilyIdx = transferFamilyIdx.value();
        }
        else
        {
            REALSIX_LOG_ERROR("No present queue found for the swap chain!");
        }
        vkGetDeviceQueue(mDevice->GetLogicDevice(), mPresentFamilyIdx, 0, &mPresentQueue);
    }

    void GfxVulkanSwapChain::CleanUpResource()
    {
        for (auto &texture : mSwapChainColorBackTextures)
        {
            SAFE_DELETE(texture);
        }
        if (GfxConfig::GetInstance().GetMsaa() > Msaa::X1)
        {
            mColorBackTexture.reset();
        }
        mDepthBackTexture.reset();
        vkDestroySwapchainKHR(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    void GfxVulkanSwapChain::Resize(Vector2u32 extent)
    {
        Resize(extent.x, extent.y);
    }

    void GfxVulkanSwapChain::Resize(uint32_t width, uint32_t height)
    {
        mDevice->WaitIdle();

        CleanUpResource();
        CreateSwapChain();
        CreateBackTextures();
    }
}