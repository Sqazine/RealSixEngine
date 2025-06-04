#include "IGfxBuffer.hpp"
#include "Config/Config.hpp"
#include "Core/Logger.hpp"
#include "Core/Marco.hpp"
#include "Gfx/VK/GfxVulkanBufferUtils.hpp"
namespace RealSix
{
    GfxVertexBuffer *GfxVertexBuffer::Create(IGfxDevice *device, const GfxBufferDesc &desc)
    {
        GfxVertexBuffer *vertexBuffer = new GfxVertexBuffer();
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            vertexBuffer->mGfxBuffer.reset(GfxVulkanBufferUtils::CreateVertexBuffer(device, desc));
            return vertexBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }

    GfxIndexBuffer *GfxIndexBuffer::Create(IGfxDevice *device, const GfxBufferDesc &desc)
    {
        GfxIndexBuffer *indexBuffer = new GfxIndexBuffer();
        indexBuffer->mElementCount = desc.bufferSize / desc.elementSize;

        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            indexBuffer->mGfxBuffer.reset(GfxVulkanBufferUtils::CreateIndexBuffer(device, desc));
            return indexBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }

    GfxUniformBuffer *GfxUniformBuffer::Create(IGfxDevice *device, const GfxBufferDesc &desc)
    {
        GfxUniformBuffer *uniformBuffer = new GfxUniformBuffer();
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            uniformBuffer->mGfxBuffer.reset(GfxVulkanBufferUtils::CreateUniformBuffer(device, desc));
            return uniformBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }

    void GfxUniformBuffer::SetData(const GfxBufferDesc &desc)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            auto vulkanGfxBuffer = static_cast<GfxVulkanBuffer *>(mGfxBuffer.get());
            GfxVulkanBufferUtils::SetBufferData(vulkanGfxBuffer, desc.bufferSize, desc.data);
            break;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }
    }

    GfxShaderStorageBuffer *GfxShaderStorageBuffer::Create(IGfxDevice *device, const GfxBufferDesc &desc)
    {
        GfxShaderStorageBuffer *storageBuffer = new GfxShaderStorageBuffer();

        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            storageBuffer->mGfxBuffer.reset(GfxVulkanBufferUtils::CreateShaderStorageBuffer(device, desc));
            return storageBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }
}