#pragma once
#include <string_view>
#include "Core/Marco.h"

namespace RealSix::FileSystem
{
    bool Exists(STRING_VIEW path);
    void CreateDirectory(STRING_VIEW path);
}