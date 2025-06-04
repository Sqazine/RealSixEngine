#pragma once
#include <vector>
#include "Gfx/IGfxDevice.hpp"
#include "GfxVulkanDevice.hpp"
#include "GfxVulkanBuffer.hpp"
namespace RealSix::GfxVulkanBufferUtils
{
    inline GfxVulkanBuffer *CreateStagingBuffer(IGfxDevice *device, float bufferSize)
    {
        GfxVulkanBuffer *stagingBuffer = new GfxVulkanBuffer(device, bufferSize,
                                                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        return stagingBuffer;
    }

    inline void SetBufferData(GfxVulkanBuffer *buffer, size_t size, const void *data)
    {
        if (buffer->IsCpuBuffer() && data != nullptr && size > 0)
        {
            auto device = buffer->GetDevice()->GetLogicDevice();
            void *bufferAddress = nullptr;
            vkMapMemory(device, buffer->GetMemory(), 0, size, 0, &bufferAddress);
            std::memcpy(bufferAddress, data, size);
            vkUnmapMemory(device, buffer->GetMemory());
        }
    }

    inline GfxVulkanBuffer *CreateVertexBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc)
    {
        std::unique_ptr<GfxVulkanBuffer> stagingBuffer;
        stagingBuffer.reset(CreateStagingBuffer(device, gfxDesc.bufferSize));

        SetBufferData(stagingBuffer.get(), (size_t)gfxDesc.bufferSize, gfxDesc.data);

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, gfxDesc.bufferSize,
                                                      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, GfxCommandType::TRANSFER, true);
        commandBuffer->Begin()
            ->CopyBuffer(stagingBuffer.get(), result, stagingBuffer->GetSize())
            ->End()
            ->Submit();

        stagingBuffer.reset();
        return result;
    }

    inline GfxVulkanBuffer *CreateIndexBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc)
    {
        std::unique_ptr<GfxVulkanBuffer> stagingBuffer;
        stagingBuffer.reset(CreateStagingBuffer(device, gfxDesc.bufferSize));

        SetBufferData(stagingBuffer.get(), (size_t)gfxDesc.bufferSize, gfxDesc.data);

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, gfxDesc.bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, GfxCommandType::TRANSFER, true);
        commandBuffer->Begin()
            ->CopyBuffer(stagingBuffer.get(), result, stagingBuffer->GetSize())
            ->End()
            ->Submit();

        stagingBuffer.reset();
        return result;
    }

    inline GfxVulkanBuffer *CreateUniformBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc)
    {
        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, gfxDesc.bufferSize,
                                                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        SetBufferData(result, (size_t)gfxDesc.bufferSize, gfxDesc.data);
        return result;
    }

    inline GfxVulkanBuffer *CreateShaderStorageBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc)
    {
        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, gfxDesc.bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (gfxDesc.data != nullptr)
        {
            std::unique_ptr<GfxVulkanBuffer> stagingBuffer;
            stagingBuffer.reset(CreateStagingBuffer(device, gfxDesc.bufferSize));

            SetBufferData(stagingBuffer.get(), (size_t)gfxDesc.bufferSize, gfxDesc.data);

            std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, GfxCommandType::TRANSFER, true);
            commandBuffer->Begin()
                ->CopyBuffer(stagingBuffer.get(), result, stagingBuffer->GetSize())
                ->End()
                ->Submit();

            stagingBuffer.reset();
        }
        return result;
    }
}