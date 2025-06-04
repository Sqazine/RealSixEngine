#include "GfxVulkanSyncObject.hpp"
#include "GfxVulkanUtils.hpp"
#include "GfxVulkanDevice.hpp"
namespace RealSix
{
    GfxVulkanFence::GfxVulkanFence(IGfxDevice *device, bool signalAtCreate)
        : GfxVulkanObject(device)
    {
        VkFenceCreateInfo info;
        ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        info.pNext = nullptr;
        info.flags = signalAtCreate ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        VK_CHECK(vkCreateFence(mDevice->GetLogicDevice(), &info, nullptr, &mHandle));
    }

    GfxVulkanFence::~GfxVulkanFence()
    {
        Wait();
        Reset();
        vkDestroyFence(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    const VkFence &GfxVulkanFence::GetHandle() const
    {
        return mHandle;
    }

    void GfxVulkanFence::Wait(bool waitAll, uint64_t timeout)
    {
        VK_CHECK(vkWaitForFences(mDevice->GetLogicDevice(), 1, &mHandle, waitAll, timeout));
    }

    void GfxVulkanFence::Reset()
    {
        VK_CHECK(vkResetFences(mDevice->GetLogicDevice(), 1, &mHandle));
    }

    bool GfxVulkanFence::IsSignaled()
    {
        auto result = vkGetFenceStatus(mDevice->GetLogicDevice(), mHandle);
        switch (result)
        {
        case VK_SUCCESS:
            return true;
        case VK_NOT_READY:
            return false;
        case VK_ERROR_DEVICE_LOST:
            REALSIX_LOG_ERROR("Device Lost!");
            return false;
        }

        return false;// for avoiding compiler warning
    }

    GfxVulkanSemaphore::GfxVulkanSemaphore(IGfxDevice *device)
        : GfxVulkanObject(device), mHandle(VK_NULL_HANDLE)
    {
        VkSemaphoreCreateInfo info = {};
        ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
        info.pNext = nullptr;
        info.flags = 0;

        VK_CHECK(vkCreateSemaphore(mDevice->GetLogicDevice(), &info, nullptr, &mHandle));
    }
    GfxVulkanSemaphore::~GfxVulkanSemaphore()
    {
        mDevice->WaitIdle();
        vkDestroySemaphore(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    const VkSemaphore &GfxVulkanSemaphore::GetHandle() const
    {
        return mHandle;
    }
}