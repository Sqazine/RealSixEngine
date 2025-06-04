#include "IGfxCommon.hpp"
#include "Core/Logger.hpp"
namespace RealSix
{
    uint8_t GetFormatByteCount(GfxFormat format)
    {
        switch (format)
        {
        case GfxFormat::R8_UNORM:
            return 1;
        case GfxFormat::R8G8_UNORM:
            return 2;
        case GfxFormat::R8G8B8_UNORM:
            return 3;
        case GfxFormat::R8G8B8A8_UNORM:
        case GfxFormat::R8G8B8A8_SRGB:
        case GfxFormat::D32_SFLOAT:
        case GfxFormat::D24_UNORM_S8_UINT:
            return 4;
        case GfxFormat::R32G32_SFLOAT:
            return 8;
        case GfxFormat::R32G32B32_SFLOAT:
            return 12;
        case GfxFormat::R32G32B32A32_SFLOAT:
        case GfxFormat::R32G32B32A32_UINT:
            return 16;
        case GfxFormat::D32_SFLOAT_S8_UINT:
            return 5;
        }

        REALSIX_LOG_ERROR("Unknown format!");
    }
}