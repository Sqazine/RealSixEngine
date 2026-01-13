#include "IGfxShader.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "Gfx/VK/GfxVulkanShader.hpp"
namespace RealSix
{
    IGfxShader *IGfxShader::AddMarco(StringView marco)
    {
        auto iter = std::find(mMarcos.begin(), mMarcos.end(), marco);
        if (iter == mMarcos.end())
            mMarcos.emplace_back(marco);
        return this;
    }

    IGfxVertexRasterShader *IGfxVertexRasterShader::Create(IGfxDevice *device)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanVertexRasterShader(device);
            return static_cast<IGfxVertexRasterShader *>(shader);
        }
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

    IGfxVertexRasterShader *IGfxVertexRasterShader::SetVertexShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::Vertex)] = source;
        return this;
    }

    IGfxVertexRasterShader *IGfxVertexRasterShader::SetFragmentShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::Fragment)] = source;
        return this;
    }

    IGfxVertexRasterShader *IGfxVertexRasterShader::SetTessellationControlShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::TessellationControl)] = source;
        return this;
    }

    IGfxVertexRasterShader *IGfxVertexRasterShader::SetTessellationEvaluationShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::TessellationEvaluation)] = source;
        return this;
    }

    IGfxVertexRasterShader *IGfxVertexRasterShader::SetGeometryShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::Geometry)] = source;
        return this;
    }

    IGfxMeshTaskRasterShader *IGfxMeshTaskRasterShader::Create(IGfxDevice *device)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            // TODO
        }
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

    IGfxMeshTaskRasterShader *IGfxMeshTaskRasterShader::SetTaskShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::Task)] = source;
        return this;
    }

    IGfxMeshTaskRasterShader *IGfxMeshTaskRasterShader::SetMeshShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::Mesh)] = source;
        return this;
    }

    IGfxMeshTaskRasterShader *IGfxMeshTaskRasterShader::SetFragmentShader(StringView source)
    {
        mShaderSources[static_cast<uint8_t>(Slot::Fragment)] = source;
        return this;
    }

    IGfxComputeShader *IGfxComputeShader::Create(IGfxDevice *device)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanComputeShader(device);
            return static_cast<IGfxComputeShader *>(shader);
        }
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
    IGfxComputeShader *IGfxComputeShader::SetComputeShader(StringView source)
    {
        mShaderSource = source;
        return this;
    }
}