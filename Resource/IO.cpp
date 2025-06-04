#include "IO.hpp"
#include <fstream>
#include <sstream>
#include "Core/Logger.hpp"
#include "Core/Common.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>
namespace RealSix
{
    String ReadTextFile(StringView path)
    {
        std::ifstream file;
        file.open(path.CString(), std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }

    String ReadUnicodeTextFile(StringView path)
    {
        std::ifstream file;

        file.open(path.CString(), std::ios::in | std::ios::binary);

        if (!file.is_open())
        {
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }

    void WriteBinaryFile(StringView path, StringView content)
    {
        std::ofstream file;
        file.open(path.CString(), std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
        }
        file.write((const char *)content.CString(), content.Size());
        file.close();
    }

       void WriteBinaryFile(StringView path, const std::vector<uint8_t> &content)
    {
        std::ofstream file;
        file.open(path.CString(), std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
        }
        file.write((const char *)content.data(), content.size());
        file.close();
    }

    String ReadBinaryFile(StringView path)
    {
        std::ifstream file;
        file.open(path.CString(), std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            REALSIX_LOG_ERROR("Failed to open file:{}", path);
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }

    GfxTextureDesc ReadTexture(StringView path)
    {
        stbi_uc *pixels = nullptr;
        int texWidth, texHeight, texChannels;
        pixels = stbi_load(path.CString(), &texWidth, &texHeight, &texChannels, STBI_default);
        auto mipLevelCount = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

        if (!pixels)
        {
            REALSIX_LOG_ERROR("Failed to load texture image:{}", path);
        }

        GfxTextureDesc result;
        result.data = static_cast<void *>(pixels);
        result.width = static_cast<uint32_t>(texWidth);
        result.height = static_cast<uint32_t>(texHeight);
        result.maxMipLevelCount = mipLevelCount;

        switch (texChannels)
        {
        case 1:
            result.format = GfxFormat::R8_UNORM;
            break;
        case 2:
            result.format = GfxFormat::R8G8_UNORM;
            break;
        case 3:
            result.format = GfxFormat::R8G8B8_UNORM;
            break;
        case 4:
            result.format = GfxFormat::R8G8B8A8_UNORM;
            break;
        default:
            REALSIX_LOG_ERROR("Unknown texture comp.", path);
            break;
        }

        return result;
    }
}