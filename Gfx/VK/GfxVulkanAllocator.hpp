#pragma once
#include <vector>
#include "Gfx/IGfxDevice.hpp"
#include "GfxVulkanDevice.hpp"
#include "GfxVulkanBuffer.hpp"
namespace RealSix::GfxVulkanAllocator
{
    GfxVulkanBuffer *CreateStagingBuffer(IGfxDevice *device, size_t bufferSize, const void *data = nullptr);

    void SetBufferData(GfxVulkanBuffer *buffer, size_t size, const void *data);

    GfxVulkanBuffer *CreateVertexBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc);

    GfxVulkanBuffer *CreateIndexBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc);

    GfxVulkanBuffer *CreateUniformBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc);

    GfxVulkanBuffer *CreateShaderStorageBuffer(IGfxDevice *device, const GfxBufferDesc &gfxDesc);
}