#include "GfxVulkanBuffer.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
namespace RealSix
{
    GfxVulkanBuffer::GfxVulkanBuffer(IGfxDevice *device, const GfxVulkanBufferDesc &desc)
        : GfxVulkanObject(device), mDesc(desc)
    {
        auto rawDevice = mDevice->GetLogicDevice();

        VkBufferCreateInfo bufferInfo;
        ZeroVulkanStruct(bufferInfo, VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);
        bufferInfo.pNext = nullptr;
        bufferInfo.size = mDesc.size;
        bufferInfo.usage = mDesc.usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK(vkCreateBuffer(rawDevice, &bufferInfo, nullptr, &mHandle));

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(rawDevice, mHandle, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->FindMemoryType(memRequirements.memoryTypeBits, mDesc.properties);

        VK_CHECK(vkAllocateMemory(rawDevice, &allocInfo, nullptr, &mMemory));

        vkBindBufferMemory(rawDevice, mHandle, mMemory, 0);
    }

    GfxVulkanBuffer::~GfxVulkanBuffer()
    {
        mDevice->WaitIdle();
        auto device = mDevice->GetLogicDevice();
        vkDestroyBuffer(device, mHandle, nullptr);
        vkFreeMemory(device, mMemory, nullptr);
    }

    BufferType GfxVulkanBuffer::GetBufferType() const
    {
        if (mDesc.properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            return BufferType::CPU;
        else
            return BufferType::GPU;
    }
}