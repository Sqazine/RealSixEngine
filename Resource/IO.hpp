#pragma once
#include "String.hpp"
#include <vector>
#include "Gfx/IGfxTexture.hpp"
namespace RealSix
{
    String ReadTextFile(StringView path);
    String ReadUnicodeTextFile(StringView path);

    void WriteBinaryFile(StringView path, const std::vector<uint8_t> &content);
    void WriteBinaryFile(StringView path, StringView content);
    String ReadBinaryFile(StringView path);

    GfxTextureDesc ReadTexture(StringView path);
}