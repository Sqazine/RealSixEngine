#include "IGfxCommandBuffer.hpp"
#include "GfxVulkanCommandBuffer.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "Core/Marco.hpp"
namespace RealSix
{
	IGfxCommandBuffer *IGfxCommandBuffer::Create(IGfxDevice *device, GfxCommandType type, bool isSingleUse)
	{
		const GfxConfig &gfxConfig = GfxConfig::GetInstance();
		switch (gfxConfig.GetBackend())
		{
		case GfxBackend::VULKAN:
			return new GfxVulkanCommandBuffer(device, type, isSingleUse);
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