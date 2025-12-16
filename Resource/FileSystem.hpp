#pragma once
#include <cstdint>
#include <vector>
#include "String.hpp"
#include "Core/Marco.hpp"
#include "Gfx/IGfxTexture.hpp"
namespace RealSix::FileSystem
{
    bool Exists(StringView path);
    void CreateDirectory(StringView path);

    String ReadTextFile(StringView path);
    String ReadUnicodeTextFile(StringView path);

    void WriteBinaryFile(StringView path, const std::vector<uint8_t> &content);
    void WriteBinaryFile(StringView path, StringView content);
    String ReadBinaryFile(StringView path);

    GfxTextureDesc ReadTexture(StringView path);
}