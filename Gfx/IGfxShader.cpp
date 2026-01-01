#include "IGfxShader.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "Gfx/VK/GfxVulkanShader.hpp"
namespace RealSix
{
	IGfxShader *IGfxShader::AddMarco(StringView marco)
	{
        auto iter = std::find(mMarcos.begin(),mMarcos.end(),marco);
        if(iter == mMarcos.end())
            mMarcos.emplace_back(marco);
		return this;
	}


    IGfxRasterShader *IGfxRasterShader::Create(IGfxDevice *device)
    {
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
        {
            IGfxShader *shader = new GfxVulkanRasterShader(device);
            return static_cast<IGfxRasterShader *>(shader);
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

    IGfxRasterShader *IGfxRasterShader::SetVertexShader(StringView source)
    {
        mShaderSources[RasterShaderSlot::Vertex] = source;
        return this;
    }

    IGfxRasterShader *IGfxRasterShader::SetFragmentShader(StringView source)
    {
		mShaderSources[RasterShaderSlot::Fragment] = source;
		return this;
    }

    IGfxRasterShader *IGfxRasterShader::SetTessellationControlShader(StringView source)
    {
		mShaderSources[RasterShaderSlot::TessellationControl] = source;
		return this;
    }

    IGfxRasterShader *IGfxRasterShader::SetTessellationEvaluationShader(StringView source)
    {
		mShaderSources[RasterShaderSlot::TessellationEvaluation] = source;
		return this;
    }

    IGfxRasterShader *IGfxRasterShader::SetGeometryShader(StringView source)
    {
		mShaderSources[RasterShaderSlot::Geometry] = source;
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