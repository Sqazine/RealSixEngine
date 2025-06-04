#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxDevice.hpp"
#include "GfxVulkanObject.hpp"
namespace RealSix
{
    constexpr uint64_t FENCE_WAIT_TIME_OUT = UINT64_MAX;

    // CPU-GPU synchronization object wrapper
    class GfxVulkanFence : public GfxVulkanObject 
    {
    public:
        GfxVulkanFence(IGfxDevice *device, bool signalAtCreate);
        ~GfxVulkanFence();

        const VkFence &GetHandle() const;

        void Wait(bool waitAll = true, uint64_t timeout = FENCE_WAIT_TIME_OUT);
        void Reset();

        bool IsSignaled();

    private:
        VkFence mHandle;
    };

    // GPU-GPU synchronization object wrapper
    class GfxVulkanSemaphore : public GfxVulkanObject
    {
    public:
        GfxVulkanSemaphore(IGfxDevice *device);
        ~GfxVulkanSemaphore();

        const VkSemaphore &GetHandle() const;

    private:
        VkSemaphore mHandle;
    };
} // namespace RealSix