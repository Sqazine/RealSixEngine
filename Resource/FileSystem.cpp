#include "FileSystem.h"
#include <filesystem>
namespace RealSix::FileSystem
{
    bool Exists(STRING_VIEW path)
    {
        return std::filesystem::exists(path);
    }
    void CreateDirectory(STRING_VIEW path)
    {
        std::filesystem::create_directory(path);
    }
}