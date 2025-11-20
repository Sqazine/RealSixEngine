#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxBuffer.h"
#include "GfxVulkanObject.h"
#include "Gfx/IGfxDevice.h"

namespace RealSix
{
    struct GfxVulkanBufferDesc
    {
        size_t size;
        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };

    enum class BufferType
    {
        GPU,
        CPU,
    };

    class GfxVulkanBuffer : public GfxVulkanObject,
        public IGfxBuffer
    {
    public:
        GfxVulkanBuffer(IGfxDevice *device, const GfxVulkanBufferDesc &desc);
        ~GfxVulkanBuffer() override;

        VkBuffer GetHandle() const { return mHandle; }
        size_t GetSize() const { return mDesc.size; }
        VkDeviceMemory GetMemory() const { return mMemory; }

        BufferType GetBufferType() const;

    private:
        GfxVulkanBufferDesc mDesc;
        VkBuffer mHandle;
        VkDeviceMemory mMemory;
    };
}