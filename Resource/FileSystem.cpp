#include "FileSystem.hpp"
#include <filesystem>
namespace RealSix::FileSystem
{
    bool Exists(StringView path)
    {
        return std::filesystem::exists(path.CString());
    }
    void CreateDirectory(StringView path)
    {
        std::filesystem::create_directory(path.CString());
    }
}