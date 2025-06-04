#include "IGfxDevice.h"
#include <utility>
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanDevice.h"
#include "Config/Config.h"

namespace RealSix
{
	IGfxDevice *IGfxDevice::Create(Window *window)
	{
		const GfxConfig &gfxConfig = GfxConfig::GetInstance();
		switch (gfxConfig.GetBackend())
		{
		case GfxBackend::VULKAN:
		{
			// Create Vulkan device
			auto device = new GfxVulkanDevice();
			device->CreateSwapChain(window);
			return device;
		}
		case GfxBackend::D3D12:
			// Create D3D12 device
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