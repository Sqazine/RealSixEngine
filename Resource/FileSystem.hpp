#pragma once
#include "String.hpp"
#include "Core/Marco.hpp"

namespace RealSix::FileSystem
{
    bool Exists(StringView path);
    void CreateDirectory(StringView path);
}