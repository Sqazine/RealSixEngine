#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>
#include "Core/Common.hpp"
#include "Core/Logger.hpp"
#include "Gfx/IGfxDevice.hpp"
#include "Gfx/IGfxBuffer.hpp"
#include "Gfx/IGfxTexture.hpp"
#include "Gfx/IGfxPipeline.hpp"
#include "Gfx/IGfxCommon.hpp"
namespace RealSix
{
    inline StringView GetErrorCode(const VkResult result)
    {
#define CASE(r)  \
    case VK_##r: \
        return #r

        switch (result)
        {

            CASE(SUCCESS);
            CASE(NOT_READY);
            CASE(TIMEOUT);
            CASE(EVENT_SET);
            CASE(EVENT_RESET);
            CASE(INCOMPLETE);
            CASE(ERROR_OUT_OF_HOST_MEMORY);
            CASE(ERROR_OUT_OF_DEVICE_MEMORY);
            CASE(ERROR_INITIALIZATION_FAILED);
            CASE(ERROR_DEVICE_LOST);
            CASE(ERROR_MEMORY_MAP_FAILED);
            CASE(ERROR_LAYER_NOT_PRESENT);
            CASE(ERROR_EXTENSION_NOT_PRESENT);
            CASE(ERROR_FEATURE_NOT_PRESENT);
            CASE(ERROR_INCOMPATIBLE_DRIVER);
            CASE(ERROR_TOO_MANY_OBJECTS);
            CASE(ERROR_FORMAT_NOT_SUPPORTED);
            CASE(ERROR_FRAGMENTED_POOL);
            CASE(ERROR_OUT_OF_POOL_MEMORY);
            CASE(ERROR_INVALID_EXTERNAL_HANDLE);
            CASE(ERROR_SURFACE_LOST_KHR);
            CASE(ERROR_NATIVE_WINDOW_IN_USE_KHR);
            CASE(SUBOPTIMAL_KHR);
            CASE(ERROR_OUT_OF_DATE_KHR);
            CASE(ERROR_INCOMPATIBLE_DISPLAY_KHR);
            CASE(ERROR_VALIDATION_FAILED_EXT);
            CASE(ERROR_INVALID_SHADER_NV);
            CASE(ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
            CASE(ERROR_FRAGMENTATION_EXT);
            CASE(ERROR_NOT_PERMITTED_EXT);
            CASE(ERROR_INVALID_DEVICE_ADDRESS_EXT);

        default:
            return "UNKNOWN_ERROR";
        }
#undef CASE
    }

#define VK_CHECK(r)                                                                                     \
    do                                                                                                  \
    {                                                                                                   \
        VkResult vkResult = (r);                                                                        \
        if (vkResult != VK_SUCCESS)                                                                     \
            REALSIX_LOG_ERROR("[{}]File:{} Line:{}", GetErrorCode(vkResult), __FILE__, __LINE__); \
    } while (false)

#define GET_VK_INSTANCE_PFN(instance, funcName)                                                             \
    do                                                                                                      \
    {                                                                                                       \
        funcName = reinterpret_cast<PFN_##funcName>(vkGetInstanceProcAddr(instance, #funcName));            \
        if (funcName == nullptr)                                                                            \
            REALSIX_LOG_WARN("Failed to get vulkan instance extension function {}", #funcName); \
    } while (false)

#define GET_VK_DEVICE_PFN(device, funcName)                                                               \
    do                                                                                                    \
    {                                                                                                     \
        funcName = reinterpret_cast<PFN_##funcName>(vkGetDeviceProcAddr(device, #funcName));              \
        if (funcName == nullptr)                                                                          \
            REALSIX_LOG_WARN("Failed to get vulkan device extension function {}", #funcName); \
    } while (false)

    // Copy From Unreal Engine (See:Engine\Source\Runtime\VulkanRHI\Public\VulkanCommon.h)
    template <class T>
    static inline void ZeroVulkanStruct(T &Struct, int32_t VkStructureType)
    {
        static_assert(!TIsPointer<T>::Value, "Don't use a pointer!");
        static_assert(offsetof(T, sType) == 0, "Assumes sType is the first member in the Vulkan type!");
        static_assert(sizeof(T::sType) == sizeof(int32_t), "Assumed sType is compatible with int32!");
        // Horrible way to coerce the compiler to not have to know what T::sType is so we can have this header not have to include vulkan.h
        (int32_t &)Struct.sType = VkStructureType;
        memset(((uint8_t *)&Struct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
    }

    class GfxVulkanDevice;
    class GfxVulkanBuffer;
    struct GfxVertexInputBinding;

    uint32_t GetVulkanQueueFamilyIndex(const GfxVulkanDevice *device, GfxCommandType type);

    VkImageAspectFlagBits GetAspectFromFormat(GfxFormat format);

    VkFormat ToVkFormat(GfxFormat format);
    GfxFormat ToFormat(VkFormat vkFormat);
    VkFilter ToVkFilter(GfxFilter filter);
    VkSamplerAddressMode ToVkSamplerAddressMode(GfxAddressMode addressMode);

    VkIndexType ToVkIndexType(GfxIndexType type);

    VkVertexInputBindingDescription GetVulkanVertexInputBindingDescription(const GfxVertexInputBinding &desc);
    std::vector<VkVertexInputAttributeDescription> GetVulkanVertexInputAttributeDescriptions(const GfxVertexInputBinding &desc);

    VkAttachmentLoadOp ToVkAttachmentOp(GfxAttachmentLoadOp loadOp);
    VkAttachmentStoreOp ToVkAttachmentOp(GfxAttachmentStoreOp storeOp);

    VkRenderingAttachmentInfo ToVkAttachment(const GfxColorAttachment &attachment);
    VkRenderingAttachmentInfo ToVkAttachment(const GfxDepthStencilAttachment &attachment);
    VkRenderingAttachmentInfo ToVkAttachment(GfxColorAttachment *attachment);
    VkRenderingAttachmentInfo ToVkAttachment(GfxDepthStencilAttachment *attachment);

    VkPrimitiveTopology ToVkPrimitiveTopology(GfxPrimitiveTopology primitiveTopology);
    VkFrontFace ToVkFrontFace(GfxFrontFace frontFace);
    VkPolygonMode ToVkPolygonMode(GfxPolygonMode polygonMode);
    VkCullModeFlags ToVkCullMode(GfxCullMode cullMode);
    VkCompareOp ToVkCompareOp(GfxCompareOp compareOp);
} // namespace RealSix