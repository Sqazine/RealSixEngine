#include "GfxVulkanDevice.hpp"
#include <iostream>
#include "Version.hpp"
#include "Core/Logger.hpp"
#include "Platform/PlatformInfo.hpp"
#include "Core/Config.hpp"
namespace RealSix
{
#ifndef NDEBUG
	constexpr char const *gInstanceValidationLayers[] = {
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_LUNARG_monitor",
	};
#endif

	constexpr char const *gInstanceDebugExtensions[] = {
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

	constexpr char const *gDeviceCommonExtensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};

	constexpr char const *gDeviceRayTracingExtensions[] = {
		VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
	};

#ifndef NDEBUG
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
	{
		Logger::Kind loggerKind;

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			loggerKind = Logger::Kind::ERROR;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			loggerKind = Logger::Kind::WARN;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			loggerKind = Logger::Kind::INFO;
			break;
		default:
			break;
		}

		String tags;

		switch (messageType)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			tags += "[GENERAL]";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			tags += "[VALIDATION]";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			tags += "[PERFORMANCE]";
			break;
		default:
			break;
		}

		switch (pCallbackData->pObjects->objectType)
		{
		case VK_OBJECT_TYPE_INSTANCE:
			tags += "[INSTANCE]";
			break;
		case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
			tags += "[PHYSICAL_DEVICE]";
			break;
		case VK_OBJECT_TYPE_DEVICE:
			tags += "[DEVICE]";
			break;
		case VK_OBJECT_TYPE_QUEUE:
			tags += "[QUEUE]";
			break;
		case VK_OBJECT_TYPE_SEMAPHORE:
			tags += "[SEMAPHORE]";
			break;
		case VK_OBJECT_TYPE_COMMAND_BUFFER:
			tags += "[COMMAND_BUFFER]";
			break;
		case VK_OBJECT_TYPE_FENCE:
			tags += "[FENCE]";
			break;
		case VK_OBJECT_TYPE_DEVICE_MEMORY:
			tags += "[DEVICE_MEMORY]";
			break;
		case VK_OBJECT_TYPE_BUFFER:
			tags += "[BUFFER]";
			break;
		case VK_OBJECT_TYPE_IMAGE:
			tags += "[IMAGE]";
			break;
		case VK_OBJECT_TYPE_EVENT:
			tags += "[EVENT]";
			break;
		case VK_OBJECT_TYPE_QUERY_POOL:
			tags += "[QUERY_POOL]";
			break;
		case VK_OBJECT_TYPE_BUFFER_VIEW:
			tags += "[BUFFER_VIEW]";
			break;
		case VK_OBJECT_TYPE_IMAGE_VIEW:
			tags += "[IMAGE_VIEW]";
			break;
		case VK_OBJECT_TYPE_SHADER_MODULE:
			tags += "[SHADER_MODULE]";
			break;
		case VK_OBJECT_TYPE_PIPELINE_CACHE:
			tags += "[PIPELINE_CACHE]";
			break;
		case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
			tags += "[PIPELINE_LAYOUT]";
			break;
		case VK_OBJECT_TYPE_RENDER_PASS:
			tags += "[RENDER_PASS]";
			break;
		case VK_OBJECT_TYPE_PIPELINE:
			tags += "[PIPELINE]";
			break;
		case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
			tags += "[DESCRIPTOR_SET_LAYOUT]";
			break;
		case VK_OBJECT_TYPE_SAMPLER:
			tags += "[SAMPLER]";
			break;
		case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
			tags += "[DESCRIPTOR_POOL]";
			break;
		case VK_OBJECT_TYPE_DESCRIPTOR_SET:
			tags += "[DESCRIPTOR_SET]";
			break;
		case VK_OBJECT_TYPE_FRAMEBUFFER:
			tags += "[FRAMEBUFFER]";
			break;
		case VK_OBJECT_TYPE_COMMAND_POOL:
			tags += "[COMMAND_POOL]";
			break;
		case VK_OBJECT_TYPE_SURFACE_KHR:
			tags += "[SURFACE_KHR]";
			break;
		case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
			tags += "[SWAPCHAIN_KHR]";
			break;
		case VK_OBJECT_TYPE_DISPLAY_KHR:
			tags += "[DISPLAY_KHR]";
			break;
		case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
			tags += "[DISPLAY_MODE_KHR]";
			break;
		default:
			break;
		}

		Logger::Log(loggerKind, "Vulkan Valication Layer {}:{}", tags.CString(), pCallbackData->pMessage);

		return VK_FALSE;
	}
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
#endif

	GfxVulkanDevice::GfxVulkanDevice()
	{
		CreateInstance();
#ifndef NDEBUG
		if (GfxConfig::GetInstance().IsEnableGpuValidation())
			CreateDebugMessengerLayer();
#endif
		EnumeratePhysicalDevices();
		SelectPhysicalDevice();
		CreateLogicDevice();
		GetQueues();
	}

	GfxVulkanDevice::~GfxVulkanDevice()
	{
		mSwapChain.reset(nullptr);
		vkDestroyDevice(mLogicDevice, nullptr);
#ifndef NDEBUG
		if (GfxConfig::GetInstance().IsEnableGpuValidation())
			DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
		vkDestroyInstance(mInstance, nullptr);
	}

	void GfxVulkanDevice::CreateInstance()
	{
		{
			uint32_t count = 0;
			vkEnumerateInstanceLayerProperties(&count, nullptr);
			mInstanceLayers.resize(count);
			vkEnumerateInstanceLayerProperties(&count, mInstanceLayers.data());
		}
		{
			uint32_t count = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
			mInstanceExtensions.resize(count);
			vkEnumerateInstanceExtensionProperties(nullptr, &count, mInstanceExtensions.data());
		}

		VkApplicationInfo appInfo{};
		appInfo.pApplicationName = "RealSix Engine";		   // TODO: Get application name from config
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: Get application version from config
		appInfo.pEngineName = "RealSix Engine";
		appInfo.engineVersion = REALSIX_VERSION_BINARY;
		appInfo.apiVersion = VK_API_VERSION_1_4;

		VkInstanceCreateInfo instanceCreateInfo{};
		ZeroVulkanStruct(instanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);
		instanceCreateInfo.pNext = nullptr;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &appInfo;

#ifndef NDEBUG
		if (GfxConfig::GetInstance().IsEnableGpuValidation())
		{
			CheckInstanceValidationLayersIsSatisfied();
			auto debugInfo = PopulateDebugMessengerCreateInfo();

			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(CountOf(gInstanceValidationLayers));
			instanceCreateInfo.ppEnabledLayerNames = gInstanceValidationLayers;
			instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugInfo;
		}
		else
		{
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.ppEnabledLayerNames = nullptr;
		}
#else
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif

		auto instanceExtensions = GetInstanceExtensionList();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance);
	}

#ifndef NDEBUG
	void GfxVulkanDevice::CreateDebugMessengerLayer()
	{
		auto debugInfo = PopulateDebugMessengerCreateInfo();
		CreateDebugUtilsMessengerEXT(mInstance, &debugInfo, nullptr, &mDebugMessenger);
	}
#endif

	void GfxVulkanDevice::EnumeratePhysicalDevices()
	{
		uint32_t count;
		vkEnumeratePhysicalDevices(mInstance, &count, nullptr);

		std::vector<VkPhysicalDevice> physicalDeviceHandleList(count);
		vkEnumeratePhysicalDevices(mInstance, &count, physicalDeviceHandleList.data());

		mPhysicalDeviceSpecificationList.resize(count);
		for (int32_t i = 0; i < mPhysicalDeviceSpecificationList.size(); ++i)
		{
			mPhysicalDeviceSpecificationList[i] = EnumeratePhysicalDeviceSpecificationFor(physicalDeviceHandleList[i]);

			const auto &spec = mPhysicalDeviceSpecificationList[i];
		}

		DumpPhysicalDeviceSpecifications();
	}

	void GfxVulkanDevice::SelectPhysicalDevice()
	{
		if (mPhysicalDeviceSpecificationList.empty())
		{
			REALSIX_LOG_ERROR("No Vulkan physical device found!");
		}
		else if (mPhysicalDeviceSpecificationList.size() == 1)
		{
			mSelectedPhysicalDeviceIndex = 0;
			REALSIX_LOG_INFO("Only one Vulkan physical device found, using it.");
		}
		else
		{
			for (size_t i = 0; i < mPhysicalDeviceSpecificationList.size(); ++i)
			{
				const auto &spec = mPhysicalDeviceSpecificationList[i];
				if (spec.properties2.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					mSelectedPhysicalDeviceIndex = i;
					break;
				}
			}
		}

		const auto &finalSelectedSpec = mPhysicalDeviceSpecificationList[mSelectedPhysicalDeviceIndex];
		REALSIX_LOG_INFO("Selected Vulkan physical device {}: {}, Type: {}, API Version: {}, Driver Version: {}",
						 mSelectedPhysicalDeviceIndex, finalSelectedSpec.properties2.properties.deviceName, finalSelectedSpec.properties2.properties.deviceType,
						 finalSelectedSpec.properties2.properties.apiVersion, finalSelectedSpec.properties2.properties.driverVersion);
	}

	void GfxVulkanDevice::CreateLogicDevice()
	{
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		const void *pExtensionFeature = nullptr;

		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeature;
		ZeroVulkanStruct(bufferDeviceAddressFeature, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES);
		bufferDeviceAddressFeature.bufferDeviceAddress = VK_TRUE;

		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature;
		ZeroVulkanStruct(dynamicRenderingFeature, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR);
		dynamicRenderingFeature.dynamicRendering  = VK_TRUE;
		dynamicRenderingFeature.pNext = &bufferDeviceAddressFeature;

		pExtensionFeature = (const void *)&dynamicRenderingFeature;

		auto finalSelectedSpec = GetPhysicalDeviceSpec();

		const float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo deviceQueueInfo;
		ZeroVulkanStruct(deviceQueueInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
		deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueInfo.queueCount = 1;
		deviceQueueInfo.pQueuePriorities = &queuePriority;

		auto deviceExtensionList = GetDeviceExtensionList();

		VkDeviceCreateInfo deviceInfo;
		ZeroVulkanStruct(deviceInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
		deviceInfo.pNext = pExtensionFeature;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &deviceQueueInfo;
		deviceInfo.enabledExtensionCount = deviceExtensionList.size();
		deviceInfo.ppEnabledExtensionNames = deviceExtensionList.data();
		deviceInfo.pEnabledFeatures = &deviceFeatures;

		VK_CHECK(vkCreateDevice(finalSelectedSpec.handle, &deviceInfo, nullptr, &mLogicDevice));

		GET_VK_DEVICE_PFN(mLogicDevice, vkGetBufferDeviceAddressKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkCreateAccelerationStructureKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkCreateRayTracingPipelinesKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkCmdBuildAccelerationStructuresKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkGetAccelerationStructureBuildSizesKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkDestroyAccelerationStructureKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkGetRayTracingShaderGroupHandlesKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkCmdTraceRaysKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkGetAccelerationStructureDeviceAddressKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkCmdWriteAccelerationStructuresPropertiesKHR);
		GET_VK_DEVICE_PFN(mLogicDevice, vkCmdCopyAccelerationStructureKHR);
	}

	void GfxVulkanDevice::GetQueues()
	{
		const auto &finalSelectedSpec = mPhysicalDeviceSpecificationList[mSelectedPhysicalDeviceIndex];

		if (finalSelectedSpec.queueFamilyIndices.graphicsFamilyIdx.has_value())
		{
			vkGetDeviceQueue(mLogicDevice, finalSelectedSpec.queueFamilyIndices.graphicsFamilyIdx.value(), 0, &mGraphicsQueue);
		}
		if (finalSelectedSpec.queueFamilyIndices.computeFamilyIdx.has_value())
		{
			vkGetDeviceQueue(mLogicDevice, finalSelectedSpec.queueFamilyIndices.computeFamilyIdx.value(), 0, &mComputeQueue);
		}
		if (finalSelectedSpec.queueFamilyIndices.transferFamilyIdx.has_value())
		{
			vkGetDeviceQueue(mLogicDevice, finalSelectedSpec.queueFamilyIndices.transferFamilyIdx.value(), 0, &mTransferQueue);
		}
		else
		{
			REALSIX_LOG_ERROR("No present queue found for the selected physical device!");
		}
	}
	
#ifndef NDEBUG
	void GfxVulkanDevice::CheckInstanceValidationLayersIsSatisfied()
	{
		for (size_t i = 0; i < CountOf(gInstanceValidationLayers); ++i)
		{
			StringView layerName = gInstanceValidationLayers[i];

			bool layerFound = false;

			for (const auto &layerProperties : mInstanceLayers)
			{
				if (layerName == layerProperties.layerName)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				REALSIX_LOG_ERROR("Missing Vulkan Validation Layer:{}", layerName);
			}
		}
	}
#endif

	void GfxVulkanDevice::CheckRequiredInstanceExtensionsIsSatisfied(const std::vector<const char *> &extensions)
	{
		for (StringView extensionName : extensions)
		{
			bool isFound = false;

			for (const auto &extensionProperties : mInstanceExtensions)
			{
				if (extensionName == extensionProperties.extensionName)
				{
					isFound = true;
					break;
				}
			}

			if (!isFound)
			{
				REALSIX_LOG_ERROR("Missing Vulkan Instance Extension:{}", extensionName);
			}
		}
	}

	VkDebugUtilsMessengerCreateInfoEXT GfxVulkanDevice::PopulateDebugMessengerCreateInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT info = {};
		ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT);
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		info.pfnUserCallback = DebugCallbackFunc;

		return info;
	}

	std::vector<const char *> GfxVulkanDevice::GetInstanceExtensionList()
	{
		std::vector<const char *> result = PlatformInfo::GetInstance().GetVulkanPlatformInfo()->GetInstanceWindowExtensionList();
		// result.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#ifndef NDEBUG
		if (GfxConfig::GetInstance().IsEnableGpuValidation())
		{
			for (size_t i = 0; i < CountOf(gInstanceDebugExtensions); ++i)
			{
				result.emplace_back(gInstanceDebugExtensions[i]);
			}
		}
#endif

		CheckRequiredInstanceExtensionsIsSatisfied(result);
		return result;
	}

	std::vector<const char *> GfxVulkanDevice::GetDeviceExtensionList()
	{
		std::vector<const char *> result;

		for (size_t i = 0; i < CountOf(gDeviceCommonExtensions); ++i)
		{
			result.emplace_back(gDeviceCommonExtensions[i]);
		}

		for (size_t i = 0; i < CountOf(gDeviceRayTracingExtensions); ++i)
		{
			result.emplace_back(gDeviceRayTracingExtensions[i]);
		}

		return result;
	}

	PhysicalDeviceSpecification GfxVulkanDevice::EnumeratePhysicalDeviceSpecificationFor(VkPhysicalDevice device)
	{
		PhysicalDeviceSpecification result;
		result.handle = device;

		vkGetPhysicalDeviceProperties2(device, &result.properties2);
		vkGetPhysicalDeviceMemoryProperties(device, &result.memoryProperties);
		vkGetPhysicalDeviceFeatures2(device, &result.features2);

		uint32_t physicalDeviceExtCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &physicalDeviceExtCount, nullptr);
		result.extensions.resize(physicalDeviceExtCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &physicalDeviceExtCount, result.extensions.data());

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(result.handle, &queueFamilyCount, nullptr);
		result.queueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(result.handle, &queueFamilyCount, result.queueFamilyProperties.data());

		uint32_t i = 0;
		for (const auto &queueFamily : result.queueFamilyProperties)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				result.queueFamilyIndices.graphicsFamilyIdx = i;
			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
				result.queueFamilyIndices.computeFamilyIdx = i;
			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
				result.queueFamilyIndices.transferFamilyIdx = i;

			if (result.queueFamilyIndices.IsComplete())
				break;
			i++;
		}

		return result;
	}

	void GfxVulkanDevice::DumpPhysicalDeviceSpecifications()
	{
		REALSIX_LOG_INFO("Physical Devices infos:");

		for (int32_t i = 0; i < mPhysicalDeviceSpecificationList.size(); ++i)
		{
			const auto &spec = mPhysicalDeviceSpecificationList[i];
			REALSIX_LOG_INFO("Physical Device {}: {}, Type: {}, API Version: {}, Driver Version: {}",
							 i, spec.properties2.properties.deviceName, spec.properties2.properties.deviceType,
							 spec.properties2.properties.apiVersion, spec.properties2.properties.driverVersion);
		}
	}

	void GfxVulkanDevice::CreateSwapChain(Window *window)
	{
		mSwapChain = std::make_unique<GfxVulkanSwapChain>(this, window);
	}

	void GfxVulkanDevice::WaitIdle()
	{
		vkDeviceWaitIdle(GetLogicDevice());
	}

	uint32_t GfxVulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(GetPhysicalDevice(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}
		REALSIX_LOG_ERROR("failed to find suitable memory type!");
		return 0; // for avoiding compiler warning, should never reach here
	}

	VkFormat GfxVulkanDevice::FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(GetPhysicalDevice(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		REALSIX_LOG_ERROR("failed to find supported format!");
		return VK_FORMAT_UNDEFINED; // for avoiding compiler warning, should never reach here
	}

	VkFormat GfxVulkanDevice::FindDepthFormat()
	{
		return FindSupportedFormat(
			{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	void GfxVulkanDevice::BeginFrame()
	{
		mSwapChain->BeginFrame();
	}

	void GfxVulkanDevice::EndFrame()
	{
		mSwapChain->EndFrame();
	}

	IGfxCommandBuffer *GfxVulkanDevice::GetCurrentBackCommandBuffer() const
	{
		return mSwapChain->GetCurrentBackCommandBuffer();
	}

	float GfxVulkanDevice::GetMaxSamplerAnisotropy() const
	{
		return GetPhysicalDeviceSpec().properties2.properties.limits.maxSamplerAnisotropy;
	}
}