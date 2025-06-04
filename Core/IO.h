#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include "Gfx/IGfxTexture.h"
namespace RealSix
{
    namespace Utf8
    {
        inline std::string Encode(const std::wstring &str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.to_bytes(str);
        }

        inline std::wstring Decode(const std::string &str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.from_bytes(str);
        }
    }

    std::string ReadTextFile(std::string_view path);
    STRING ReadUnicodeTextFile(std::string_view path);

    void WriteBinaryFile(std::string_view path, const std::vector<uint8_t> &content);
    std::vector<uint8_t> ReadBinaryFile(std::string_view path);

    GfxTextureDesc ReadTexture(std::string_view path);
}