#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxBuffer.hpp"
#include "GfxVulkanObject.hpp"
#include "Gfx/IGfxDevice.hpp"

namespace RealSix
{

    class GfxVulkanBuffer : public GfxVulkanObject,
                            public IGfxBuffer
    {
    public:
        GfxVulkanBuffer(IGfxDevice *device, size_t size,
                        VkBufferUsageFlags usage,
                        VkMemoryPropertyFlags properties);
        ~GfxVulkanBuffer() override;

        VkBuffer GetHandle() const { return mHandle; }
        size_t GetSize() const { return mSize; }
        size_t GetAllocatedSize() const { return mAllocatedSize; }
        VkDeviceMemory GetMemory() const { return mMemory; }

        size_t GetAddress() const override { return mAddress; }

        bool IsCpuBuffer() const;

    private:
        size_t mSize;
        size_t mAllocatedSize;

        VkBufferUsageFlags mUsage;
        VkMemoryPropertyFlags mMemoryProperties;

        VkBuffer mHandle{VK_NULL_HANDLE};
        VkDeviceMemory mMemory{VK_NULL_HANDLE};

        size_t mAddress{0};
    };
}