#include "GfxVulkanUtils.hpp"
#include "GfxVulkanDevice.hpp"
#include "Gfx/IGfxCommandBuffer.hpp"
#include "Gfx/IGfxTexture.hpp"
namespace RealSix
{
    uint32_t GetVulkanQueueFamilyIndex(const GfxVulkanDevice *device, GfxCommandType type)
    {
        switch (type)
        {
        case GfxCommandType::GRAPHICS:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.graphicsFamilyIdx.value();
        case GfxCommandType::COMPUTE:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.computeFamilyIdx.value();
        case GfxCommandType::TRANSFER:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.transferFamilyIdx.value();
        default:
            REALSIX_LOG_ERROR("Unknown GfxCommandType: {}", static_cast<int>(type));
            return 0;
        }
    }

    VkImageAspectFlagBits GetAspectFromFormat(GfxFormat format)
    {
        switch (format)
        {
        case GfxFormat::D32_SFLOAT:
        case GfxFormat::D32_SFLOAT_S8_UINT:
        case GfxFormat::D24_UNORM_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        default:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

#define FormatDefines()       \
    DEF(R8_UNORM);            \
    DEF(R8G8_UNORM);          \
    DEF(R8G8B8_UNORM);        \
    DEF(R8G8B8A8_UNORM);      \
    DEF(R8_SNORM);            \
    DEF(R8G8_SNORM);          \
    DEF(R8G8B8_SNORM);        \
    DEF(R8G8B8A8_SNORM);      \
    DEF(R8_SRGB);             \
    DEF(R8G8_SRGB);           \
    DEF(R8G8B8_SRGB);         \
    DEF(R8G8B8A8_SRGB);       \
    DEF(B8G8R8A8_SRGB);       \
    DEF(B8G8R8A8_UNORM);      \
    DEF(R32G32_SFLOAT);       \
    DEF(R32G32B32_SFLOAT);    \
    DEF(R32G32B32A32_SFLOAT); \
    DEF(R32G32B32A32_UINT); \
    DEF(D24_UNORM_S8_UINT);   \
    DEF(D32_SFLOAT);          \
    DEF(D32_SFLOAT_S8_UINT);

    VkFormat ToVkFormat(GfxFormat format)
    {
#define DEF(format)           \
    case GfxFormat::##format: \
        return VK_FORMAT_##format

        switch (format)
        {
            FormatDefines();
        default:
            REALSIX_LOG_ERROR("Unsupported Format");
            return VK_FORMAT_R8_UNORM; // for avoiding compiler warning
        }
#undef DEF
    }

    GfxFormat ToFormat(VkFormat vkFormat)
    {
#define DEF(format)          \
    case VK_FORMAT_##format: \
        return GfxFormat::##format

        switch (vkFormat)
        {
            FormatDefines();
        default:
            REALSIX_LOG_ERROR("Unsupported Format");
            return GfxFormat::B8G8R8A8_UNORM; // for avoiding compiler warning
        }
#undef DEF
    }

    VkFilter ToVkFilter(GfxFilter filter)
    {
#define DEF(filter)           \
    case GfxFilter::##filter: \
        return VK_FILTER_##filter

        switch (filter)
        {
            DEF(NEAREST);
            DEF(LINEAR);
        default:
            REALSIX_LOG_ERROR("Unsupported Vulkan Filter");
            return VK_FILTER_NEAREST; // for avoiding compiler warning
        }
#undef DEF
    }

    VkSamplerAddressMode ToVkSamplerAddressMode(GfxAddressMode addressMode)
    {
#define DEF(addressMode)                \
    case GfxAddressMode::##addressMode: \
        return VK_SAMPLER_ADDRESS_MODE_##addressMode

        switch (addressMode)
        {
            DEF(REPEAT);
            DEF(MIRRORED_REPEAT);
            DEF(CLAMP_TO_EDGE);
        default:
            REALSIX_LOG_ERROR("Unsupported Vulkan Address Mode");
            return VK_SAMPLER_ADDRESS_MODE_REPEAT; // for avoiding compiler warning
        }
#undef DEF
    }

    VkIndexType ToVkIndexType(GfxIndexType type)
    {
        switch (type)
        {
        case GfxIndexType::UINT16:
            return VK_INDEX_TYPE_UINT16;
        case GfxIndexType::UINT32:
            return VK_INDEX_TYPE_UINT32;
        default:
            REALSIX_LOG_ERROR("Unsupported Vulkan index type");
            return VK_INDEX_TYPE_UINT32; // for avoiding compiler warning
        }
    }

    VkVertexInputRate GetVkVertexInput(GfxVertexInputType type)
    {
        switch (type)
        {
        case GfxVertexInputType::PER_VERTEX:
            return VK_VERTEX_INPUT_RATE_VERTEX;
        case GfxVertexInputType::PER_INSTANCE:
            return VK_VERTEX_INPUT_RATE_INSTANCE;
        default:
            REALSIX_LOG_ERROR("Unknown VertexInputType!");
        }
    }

    VkVertexInputBindingDescription GetVulkanVertexInputBindingDescription(const GfxVertexInputBinding &desc)
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = desc.bindingPoint;
        bindingDescription.stride = desc.size;
        bindingDescription.inputRate = GetVkVertexInput(desc.vertexInputType);

        return bindingDescription;
    }

    std::vector<VkVertexInputAttributeDescription> GetVulkanVertexInputAttributeDescriptions(const GfxVertexInputBinding &desc)
    {
        std::vector<VkVertexInputAttributeDescription> result;
        for (size_t i = 0; i < desc.attribs.size(); ++i)
        {
            VkVertexInputAttributeDescription attributeDescriptions;
            attributeDescriptions.binding = desc.bindingPoint;
            attributeDescriptions.location = i;
            attributeDescriptions.format = ToVkFormat(desc.attribs[i].format);
            attributeDescriptions.offset = desc.attribs[i].offset;

            result.emplace_back(attributeDescriptions);
        }

        return result;
    }

    VkAttachmentLoadOp ToVkAttachmentOp(GfxAttachmentLoadOp loadOp)
    {
        switch (loadOp)
        {
        case GfxAttachmentLoadOp::LOAD:
            return VK_ATTACHMENT_LOAD_OP_LOAD;
        case GfxAttachmentLoadOp::CLEAR:
            return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case GfxAttachmentLoadOp::DONT_CARE:
            return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        default:
            REALSIX_LOG_ERROR("Unknown AttachmentOp");
            return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
    }

    VkAttachmentStoreOp ToVkAttachmentOp(GfxAttachmentStoreOp storeOp)
    {
        switch (storeOp)
        {
        case GfxAttachmentStoreOp::STORE:
            return VK_ATTACHMENT_STORE_OP_STORE;
        case GfxAttachmentStoreOp::DONT_CARE:
            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        default:
            REALSIX_LOG_ERROR("Unknown AttachmentOp");
            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
    }

    VkRenderingAttachmentInfo ToVkAttachment(const GfxColorAttachment &attachment)
    {
        VkRenderingAttachmentInfo result;
        ZeroVulkanStruct(result, VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO);
        result.imageView = static_cast<GfxVulkanTexture *>(attachment.texture)->GetView();
        result.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        result.resolveMode = VK_RESOLVE_MODE_NONE;
        result.resolveImageView = nullptr;
        result.resolveImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        result.loadOp = ToVkAttachmentOp(attachment.loadOp);
        result.storeOp = ToVkAttachmentOp(attachment.storeOp);
        result.clearValue.color.float32[0] = attachment.clearValue.x;
        result.clearValue.color.float32[1] = attachment.clearValue.y;
        result.clearValue.color.float32[2] = attachment.clearValue.z;
        result.clearValue.color.float32[3] = attachment.clearValue.w;
        return result;
    }

    VkRenderingAttachmentInfo ToVkAttachment(const GfxDepthStencilAttachment &attachment)
    {
        VkRenderingAttachmentInfo result;
        ZeroVulkanStruct(result, VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO);
        result.imageView = static_cast<GfxVulkanTexture *>(attachment.texture)->GetView();
        result.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        result.resolveMode = VK_RESOLVE_MODE_NONE;
        result.resolveImageView = nullptr;
        result.resolveImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        result.loadOp = ToVkAttachmentOp(attachment.loadOp);
        result.storeOp = ToVkAttachmentOp(attachment.storeOp);
        result.clearValue.depthStencil.depth = attachment.depthClearValue;
        result.clearValue.depthStencil.stencil = attachment.stencilClearValue;
        return result;
    }

    VkRenderingAttachmentInfo ToVkAttachment(GfxColorAttachment *attachment)
    {
        return ToVkAttachment(*attachment);
    }
    VkRenderingAttachmentInfo ToVkAttachment(GfxDepthStencilAttachment *attachment)
    {
        return ToVkAttachment(*attachment);
    }

    VkPrimitiveTopology ToVkPrimitiveTopology(GfxPrimitiveTopology primitiveTopology)
    {
#define DEF(topology)                      \
    case GfxPrimitiveTopology::##topology: \
        return VK_PRIMITIVE_TOPOLOGY_##topology

        switch (primitiveTopology)
        {
            DEF(POINT_LIST);
            DEF(LINE_LIST);
            DEF(LINE_STRIP);
            DEF(TRIANGLE_LIST);
            DEF(TRIANGLE_STRIP);
            DEF(TRIANGLE_FAN);
            DEF(LINE_LIST_WITH_ADJACENCY);
            DEF(LINE_STRIP_WITH_ADJACENCY);
            DEF(TRIANGLE_LIST_WITH_ADJACENCY);
            DEF(TRIANGLE_STRIP_WITH_ADJACENCY);
            DEF(PATCH_LIST);
        default:
            REALSIX_LOG_ERROR("Unknown Primitive Topology!");
        }
#undef DEF
    }

    VkFrontFace ToVkFrontFace(GfxFrontFace frontFace)
    {
        switch (frontFace)
        {
        case GfxFrontFace::CLOCKWISE:
            return VK_FRONT_FACE_CLOCKWISE;
        case GfxFrontFace::COUNTER_CLOCKWISE:
            return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        default:
            REALSIX_LOG_ERROR("Unknown FrontFace!");
        }
    }

    VkPolygonMode ToVkPolygonMode(GfxPolygonMode polygonMode)
    {
        switch (polygonMode)
        {
        case GfxPolygonMode::FILL:
            return VK_POLYGON_MODE_FILL;
        case GfxPolygonMode::LINE:
            return VK_POLYGON_MODE_LINE;
        case GfxPolygonMode::POINT:
            return VK_POLYGON_MODE_POINT;
        default:
            REALSIX_LOG_ERROR("Unknown PolygonMode!");
        }
    }

    VkCullModeFlags ToVkCullMode(GfxCullMode cullMode)
    {
        switch (cullMode)
        {
        case GfxCullMode::NONE:
            return VK_CULL_MODE_NONE;
        case GfxCullMode::FRONT:
            return VK_CULL_MODE_FRONT_BIT;
        case GfxCullMode::BACK:
            return VK_CULL_MODE_BACK_BIT;
        case GfxCullMode::FRONT_AND_BACK:
            return VK_CULL_MODE_FRONT_AND_BACK;
        default:
            REALSIX_LOG_ERROR("Unknown CullMode");
        }
    }

    VkCompareOp ToVkCompareOp(GfxCompareOp compareOp)
    {
#define DEF(op)           \
    case GfxCompareOp::##op: \
        return VK_COMPARE_OP_##op

        switch (compareOp)
        {
            DEF(NEVER);
            DEF(LESS);
            DEF(EQUAL);
            DEF(LESS_OR_EQUAL);
            DEF(GREATER);
            DEF(NOT_EQUAL);
            DEF(GREATER_OR_EQUAL);
            DEF(ALWAYS);
        default:
            REALSIX_LOG_ERROR("Unknown GfxCompareOp");
        }

#undef DEF
    }
}