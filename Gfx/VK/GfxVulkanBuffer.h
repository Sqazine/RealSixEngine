#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxBuffer.h"
#include "GfxVulkanObject.h"
#include "Gfx/IGfxDevice.h"

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

        bool IsCpuBuffer() const;

    private:
        size_t mSize;
        size_t mAllocatedSize;

        VkBufferUsageFlags mUsage;
        VkMemoryPropertyFlags mMemoryProperties;

        VkBuffer mHandle;
        VkDeviceMemory mMemory;
    };
}