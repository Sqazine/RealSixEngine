#pragma once
#include <optional>
#include <vulkan/vulkan.h>
#include <memory>
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanSwapChain.h"

namespace RealSix
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamilyIdx;
		std::optional<uint32_t> computeFamilyIdx;
		std::optional<uint32_t> transferFamilyIdx;

		bool IsComplete() const
		{
			return graphicsFamilyIdx.has_value() &&
				   computeFamilyIdx.has_value() &&
				   transferFamilyIdx.has_value();
		}

		bool IsSameFamilyIndex() const
		{
			return graphicsFamilyIdx.value() == computeFamilyIdx.value() && computeFamilyIdx.value() == transferFamilyIdx.value();
		}

		std::vector<uint32_t> IndexArray() const
		{
			return {graphicsFamilyIdx.value(), computeFamilyIdx.value(), transferFamilyIdx.value()};
		}
	};

	struct PhysicalDeviceSpecification
	{
		VkPhysicalDevice handle;
		std::vector<VkExtensionProperties> deviceExtensions;
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures features;
		VkPhysicalDeviceMemoryProperties memoryProperties;
		std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		QueueFamilyIndices queueFamilyIndices;
	};

	class GfxVulkanDevice : public IGfxDevice
	{
	public:
		GfxVulkanDevice();
		~GfxVulkanDevice() override;

		void BeginFrame() override;
		void EndFrame() override;

		IGfxCommandBuffer *GetCurrentBackCommandBuffer() const override;

		VkDevice GetLogicDevice() const { return mLogicDevice; }
		VkInstance GetInstance() const { return mInstance; }

		const VkQueue &GetGraphicsQueue() const { return mGraphicsQueue; }
		const VkQueue &GetComputeQueue() const { return mComputeQueue; }
		const VkQueue &GetTransferQueue() const { return mTransferQueue; }

		const PhysicalDeviceSpecification &GetPhysicalDeviceSpec() const { return mPhysicalDeviceSpecificationList[mSelectedPhysicalDeviceIndex]; }
		VkPhysicalDevice GetPhysicalDevice() const { return GetPhysicalDeviceSpec().handle; }

		void CreateSwapChain(Window *window);

		void WaitIdle();

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();

	private:
		void CreateInstance();
#ifndef NDEBUG
		void CreateDebugMessengerLayer();
#endif
		void EnumeratePhysicalDevices();
		void SelectPhysicalDevice();
		void CreateLogicDevice();
		void GetQueues();

		std::vector<VkLayerProperties> mInstanceLayers;
		std::vector<VkExtensionProperties> mInstanceExtensions;
		VkInstance mInstance;

#ifndef NDEBUG
		const std::vector<const char *> mValidationInstanceLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_LUNARG_monitor",
		};

		const std::vector<const char *> mDebugRequiredInstanceExtensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
		std::vector<PhysicalDeviceSpecification> mPhysicalDeviceSpecificationList;
		size_t mSelectedPhysicalDeviceIndex = 0;

		const std::vector<const char *> mDeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		VkDevice mLogicDevice;

		VkQueue mGraphicsQueue;
		VkQueue mComputeQueue;
		VkQueue mTransferQueue;

	private:
#ifndef NDEBUG
		void CheckInstanceValidationLayersIsSatisfied();
#endif
		void CheckRequiredInstanceExtensionsIsSatisfied(const std::vector<const char *> &extensions);

		VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();
		std::vector<const char *> GetRequiredInstanceExtensions();
		PhysicalDeviceSpecification EnumeratePhysicalDeviceSpecificationFor(VkPhysicalDevice device);

		void DumpPhysicalDeviceSpecifications();
	};
}