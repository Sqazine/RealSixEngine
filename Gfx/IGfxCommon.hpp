#pragma once
#include <cstdint>
#include <type_traits>
#include "Core/Marco.hpp"
namespace RealSix
{
    enum class GfxFormat
    {
        R8_UNORM,
        R8G8_UNORM,
        R8G8B8_UNORM,
        R8G8B8A8_UNORM,

        R8_SNORM,
        R8G8_SNORM,
        R8G8B8_SNORM,
        R8G8B8A8_SNORM,

        R8_SRGB,
        R8G8_SRGB,
        R8G8B8_SRGB,
        R8G8B8A8_SRGB,

        B8G8R8A8_SRGB,
        B8G8R8A8_UNORM,

        R32G32_SFLOAT,
        R32G32B32_SFLOAT,
        R32G32B32A32_SFLOAT,

        R32G32B32A32_UINT,

        D32_SFLOAT,
        D32_SFLOAT_S8_UINT,
        D24_UNORM_S8_UINT
    };

    uint8_t GetFormatByteCount(GfxFormat format);

    enum class GfxFilter
    {
        NEAREST,
        LINEAR
    };

    enum class GfxAddressMode
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
    };

    enum class GfxAttachmentLoadOp
    {
        LOAD,
        CLEAR,
        DONT_CARE,
    };

    enum class GfxAttachmentStoreOp
    {
        STORE,
        DONT_CARE,
    };

    enum class ColorChannelMask : uint32_t
    {
        R = 0x00000001,
        G = 0x00000002,
        B = 0x00000004,
        A = 0x00000008
    };

#define COLOR_CHANNEL_MASK_CAST(t) (static_cast<uint32_t>(std::underlying_type<ColorChannelMask>::type(t)))
    ENUM_CLASS_OP(COLOR_CHANNEL_MASK_CAST, ColorChannelMask);

    enum class GfxPrimitiveTopology
    {
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        LINE_LIST_WITH_ADJACENCY,
        LINE_STRIP_WITH_ADJACENCY,
        TRIANGLE_LIST_WITH_ADJACENCY,
        TRIANGLE_STRIP_WITH_ADJACENCY,
        PATCH_LIST,
    };

    enum class GfxFrontFace
    {
        COUNTER_CLOCKWISE,
        CLOCKWISE,
    };

    enum class GfxPolygonMode
    {
        FILL,
        LINE,
        POINT
    };

    enum class GfxCullMode
    {
        NONE,
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    enum class GfxVertexInputType
    {
        PER_VERTEX,
        PER_INSTANCE,
    };

    enum class GfxIndexType
    {
        UINT16,
        UINT32
    };

    enum class GfxCommandType
    {
        GRAPHICS,
        COMPUTE,
        TRANSFER
    };

    enum class GfxCompareOp
    {
        NEVER,
        LESS,
        EQUAL,
        LESS_OR_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_OR_EQUAL,
        ALWAYS,
    };
}