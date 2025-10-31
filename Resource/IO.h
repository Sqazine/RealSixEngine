#pragma once
#include <string>
#include <vector>
#include "Gfx/IGfxTexture.h"
namespace RealSix
{
    std::string ReadTextFile(std::string_view path);
    STRING ReadUnicodeTextFile(std::string_view path);

    void WriteBinaryFile(std::string_view path, const std::vector<uint8_t> &content);
    std::vector<uint8_t> ReadBinaryFile(std::string_view path);

    GfxTextureDesc ReadTexture(std::string_view path);
}