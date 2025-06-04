#include "IGfxCommandBuffer.h"
#include "GfxVulkanCommandBuffer.h"
#include "Config/Config.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
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