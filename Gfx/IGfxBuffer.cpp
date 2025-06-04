#include "IGfxBuffer.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "Gfx/VK/GfxVulkanBufferCommon.h"
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
            vertexBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateVertexBuffer(device, desc));
            return vertexBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
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
            uniformBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateUniformBuffer(device, desc));
            return uniformBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
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
            GfxVulkanBufferCommon::SetCpuBufferData(vulkanGfxBuffer, desc.bufferSize, desc.data);
            break;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }
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
            indexBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateIndexBuffer(device, desc));
            return indexBuffer;
        }
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
        return nullptr; // for avoiding compiler warning
    }
}