#pragma once
#include <optional>
#include <vulkan/vulkan.h>
#include <memory>
#include "Gfx/IGfxDevice.hpp"
#include "GfxVulkanUtils.hpp"
#include "GfxVulkanSwapChain.hpp"

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
		PhysicalDeviceSpecification()
		{
			ZeroVulkanStruct(properties2, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2);
			ZeroVulkanStruct(features2, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2);
		}

		VkPhysicalDevice handle;
		std::vector<VkExtensionProperties> extensions;
		VkPhysicalDeviceProperties2 properties2;
		VkPhysicalDeviceFeatures2 features2;
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

		float GetMaxSamplerAnisotropy() const;

		// Extension Functions
		PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR{nullptr};
		PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR{nullptr};
		PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR{nullptr};
		PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR{nullptr};
		PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR{nullptr};
		PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR{nullptr};
		PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR{nullptr};
		PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR{nullptr};
		PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR{nullptr};
		PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR{nullptr};
		PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR{nullptr};

	private:
		void CreateInstance();
		void EnumeratePhysicalDevices();
		void SelectPhysicalDevice();
		void CreateLogicDevice();
		void GetQueues();

		void CheckRequiredInstanceExtensionsIsSatisfied(const std::vector<const char *> &extensions);

		VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();
		PhysicalDeviceSpecification EnumeratePhysicalDeviceSpecificationFor(VkPhysicalDevice device);

		std::vector<const char *> GetInstanceExtensionList();
		std::vector<const char *> GetDeviceExtensionList();

		void DumpPhysicalDeviceSpecifications();

		std::vector<VkLayerProperties> mInstanceLayers;
		std::vector<VkExtensionProperties> mInstanceExtensions;
		VkInstance mInstance;

#ifndef NDEBUG
		void CheckInstanceValidationLayersIsSatisfied();
		void CreateDebugMessengerLayer();

		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
		std::vector<PhysicalDeviceSpecification> mPhysicalDeviceSpecificationList;
		size_t mSelectedPhysicalDeviceIndex = 0;

		VkDevice mLogicDevice;

		VkQueue mGraphicsQueue;
		VkQueue mComputeQueue;
		VkQueue mTransferQueue;
	};
}