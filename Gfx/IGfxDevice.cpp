#include "IGfxDevice.hpp"
#include <utility>
#include "Core/Logger.hpp"
#include "Gfx/VK/GfxVulkanDevice.hpp"
#include "Core/Config.hpp"

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