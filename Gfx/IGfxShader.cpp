#include "IGfxShader.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanShader.h"
namespace RealSix
{
    IGfxRasterShader *IGfxRasterShader::Create(IGfxDevice *device,
                                               std::string_view vertContent,
                                               std::string_view fragContent,
                                               std::string_view tessCtrlContent,
                                               std::string_view tessEvalContent,
                                               std::string_view geomContent)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanRasterShader(device, vertContent, fragContent, tessCtrlContent, tessEvalContent, geomContent);
            return static_cast<IGfxRasterShader *>(shader);
        }
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

    IGfxRasterShader *IGfxRasterShader::Create(IGfxDevice *device,
                                               const std::vector<uint8_t> &vertContent,
                                               const std::vector<uint8_t> &fragContent,
                                               const std::vector<uint8_t> &tessCtrlContent,
                                               const std::vector<uint8_t> &tessEvalContent,
                                               const std::vector<uint8_t> &geomContent)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanRasterShader(device, vertContent, fragContent, tessCtrlContent, tessEvalContent, geomContent);
            return static_cast<IGfxRasterShader *>(shader);
        }
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

    IGfxComputeShader *IGfxComputeShader::Create(IGfxDevice *device,
                                                 std::string_view compContent)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanComputeShader(device, compContent);
            return static_cast<IGfxComputeShader *>(shader);
        }
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

    IGfxComputeShader *IGfxComputeShader::Create(IGfxDevice *device,
                                                 const std::vector<uint8_t> &compContent)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanComputeShader(device, compContent);
            return static_cast<IGfxComputeShader *>(shader);
        }
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