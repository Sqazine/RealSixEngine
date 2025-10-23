#include "IGfxPipeline.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanPipeline.h"
namespace RealSix
{
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
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
        return nullptr;
    }


    IGfxComputePipeline::IGfxComputePipeline(const GfxComputePipelineStateDesc &pipelineState)
        : mPipelineStateDesc(pipelineState)
    {
    }

    IGfxComputePipeline *IGfxComputePipeline::Create(IGfxDevice *device, const GfxComputePipelineStateDesc &pipelineState)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanComputePipeline(device, pipelineState);
        case GfxBackend::D3D12:
            REALSIX_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
            break;
        }

        REALSIX_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.GetBackend()));
        return nullptr;
    }
}