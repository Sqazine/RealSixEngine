#include "IO.h"
#include <fstream>
#include <sstream>
#include "Logger/Logger.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>
namespace RealSix
{
    std::string ReadTextFile(std::string_view path)
    {
        std::ifstream file;
        file.open(path.data(), std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
#ifdef REALSIX_UTF8_ENCODE
            STRING utf8Path = Utf8::Decode(path.data());
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), utf8Path);
#else
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), path);
#endif
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }

    STRING ReadUnicodeTextFile(std::string_view path)
    {
        STD_IFSTREAM file;

#ifdef REALSIX_UTF8_ENCODE
        STRING utf8Path = Utf8::Decode(path.data());
        file.open(utf8Path.c_str(), std::ios::in | std::ios::binary);
#else
        file.open(path.c_str(), std::ios::in | std::ios::binary);
#endif
        if (!file.is_open())
        {
#ifdef REALSIX_UTF8_ENCODE
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), utf8Path);
#else
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), path);
#endif
        }

        STRING_STREAM sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }

    void WriteBinaryFile(std::string_view path, const std::vector<uint8_t> &content)
    {
        std::ofstream file;
        file.open(path.data(), std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
#ifdef REALSIX_UTF8_ENCODE
            auto utf8Path = Utf8::Decode(path.data());
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), utf8Path);
#else
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), path);
#endif
        }
        file.write((const char *)content.data(), content.size());
        file.close();
    }

    std::vector<uint8_t> ReadBinaryFile(std::string_view path)
    {
        std::ifstream file;
        file.open(path.data(), std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
#ifdef REALSIX_UTF8_ENCODE
            auto utf8Path = Utf8::Decode(path.data());
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), utf8Path);
#else
            REALSIX_LOG_ERROR(TEXT("Failed to open file:{}"), path);
#endif
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        auto str = sstream.str();

        std::vector<uint8_t> result;
        result.assign(str.begin(), str.end());

        return result;
    }

    GfxTextureDesc ReadTexture(std::string_view path)
    {
        stbi_uc *pixels = nullptr;
        int texWidth, texHeight, texChannels;
        pixels = stbi_load(path.data(), &texWidth, &texHeight, &texChannels, STBI_default);
        auto mipLevelCount = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

        if (!pixels)
        {
#ifdef REALSIX_UTF8_ENCODE
            STRING utf8Path = Utf8::Decode(path.data());
            REALSIX_LOG_ERROR(TEXT("Failed to load texture image:{}"), utf8Path);
#else
            REALSIX_LOG_ERROR(TEXT("Failed to load texture image:{}"), path);
#endif
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
#ifdef REALSIX_UTF8_ENCODE
            STRING utf8PPath = Utf8::Decode(path.data());
            REALSIX_LOG_ERROR(TEXT("Unknown texture comp."), utf8PPath);
#else
            REALSIX_LOG_ERROR(TEXT("Unknown texture comp."), path);
#endif
            break;
        }

        return result;
    }
}