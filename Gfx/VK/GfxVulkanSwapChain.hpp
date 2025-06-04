#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "Math/Vector2.hpp"
#include "GfxVulkanSyncObject.hpp"
#include "GfxVulkanObject.hpp"
#include "GfxVulkanCommandBuffer.hpp"
#include "GfxVulkanTexture.hpp"
#include "GfxVulkanUtils.hpp"
namespace RealSix
{
    struct SwapChainDetails
    {
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
    };

    class GfxVulkanSwapChain : public GfxVulkanObject, public IGfxSwapChain
    {
    public:
        GfxVulkanSwapChain(IGfxDevice *device, Window *window);
        ~GfxVulkanSwapChain() override;

        void BeginFrame() override;
        void EndFrame() override;
        IGfxCommandBuffer *GetCurrentBackCommandBuffer() const override;

        uint8_t GetBackBufferCount() const override;

        const VkSwapchainKHR &GetHandle() const { return mHandle; }
        uint8_t GetCurrentBackBufferIndex() const;
        VkExtent2D GetExtent() const;
        const VkSurfaceFormatKHR GetSurfaceFormat() const;
        uint32_t GetNextFrameIndex() const;
        GfxVulkanTexture *GetCurrentSwapChainBackTexture() const;
        VkFormat GetColorTextureFormat() const { return mSurfaceFormat.format; }
        VkFormat GetDepthTextureFormat() const { return ToVkFormat(mDepthBackTexture->GetDesc().format); }
    private:
        void CreateSurface();
        void ObtainPresentQueue();
        void CreateSwapChain();
        void CreateBackTextures();
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void SetupAttachments();

        SwapChainDetails QuerySwapChainDetails();

        VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void Resize(Vector2u32 extent);
        void Resize(uint32_t width, uint32_t height);
        void Present(const GfxVulkanSemaphore *waitFor = nullptr);

        void CleanUpResource();

        uint32_t mPresentFamilyIdx{0};
        VkQueue mPresentQueue{VK_NULL_HANDLE};
        VkSurfaceKHR mSurface;
        VkSwapchainKHR mHandle;

        VkSurfaceFormatKHR mSurfaceFormat;
        VkExtent2D mExtent;
        VkPresentModeKHR mPresentMode;

        uint32_t mSwapChainImageCount{0};
        uint32_t mFrameOverlapCount{0};

        uint32_t mCurrentFrameIndex{0};
        uint32_t mNextFrameIndex{0};

        std::vector<std::unique_ptr<GfxVulkanCommandBuffer>> mGfxCommandBuffer;
        std::vector<std::unique_ptr<GfxVulkanSemaphore>> mPresentSemaphore;

        std::vector<GfxVulkanTexture *> mSwapChainColorBackTextures;

        std::unique_ptr<GfxVulkanTexture> mColorBackTexture;
        std::unique_ptr<GfxVulkanTexture> mDepthBackTexture;
    };
}