#include "IGfxPipeline.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "Gfx/VK/GfxVulkanPipeline.hpp"
namespace RealSix
{
    bool operator==(const GfxVertexAttribute &lhs, const GfxVertexAttribute &rhs)
    {
        return lhs.name == rhs.name &&
               lhs.format == rhs.format &&
               lhs.offset == rhs.offset;
    }
    bool operator!=(const GfxVertexAttribute &lhs, const GfxVertexAttribute &rhs)
    {
        return !!(lhs == rhs);
    }
    bool operator==(const GfxVertexInputBinding &lhs, const GfxVertexInputBinding &rhs)
    {
        return lhs.bindingPoint == rhs.bindingPoint &&
               lhs.size == rhs.size &&
               lhs.vertexInputType == rhs.vertexInputType &&
               lhs.attribs == rhs.attribs;
    }

    bool operator!=(const GfxVertexInputBinding &lhs, const GfxVertexInputBinding &rhs)
    {
        return !(lhs == rhs);
    }

    IGfxRasterPipeline::IGfxRasterPipeline(const GfxRasterPipelineStateDesc &pipelineState)
        : mPipelineStateDesc(pipelineState)
    {
    }

    IGfxRasterPipeline *IGfxRasterPipeline::Create(IGfxDevice *device, const GfxRasterPipelineStateDesc &pipelineState)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanRasterPipeline(device, pipelineState);
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr;
    }

    IGfxComputePipeline::IGfxComputePipeline(IGfxComputeShader *shader)
        : mShader(shader)
    {
    }

    IGfxComputePipeline *IGfxComputePipeline::Create(IGfxDevice *device, IGfxComputeShader *shader)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanComputePipeline(device, shader);
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR("Not implemented D3D12 device creation yet");
            break;
        default:
            REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR("Unreachable GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
        return nullptr;
    }
}