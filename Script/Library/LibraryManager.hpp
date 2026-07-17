#pragma once
#include <unordered_map>
#include <functional>
#include "Value.hpp"
#include "Object.hpp"
#include "Common.hpp"
#include "Core/Common.hpp"
namespace RealSix::Script
{
    class REALSIX_API LibraryManager:public Singleton<LibraryManager>
    {
    public:
        void Init();
        void CleanUp();

        void RegisterLibrary(ModuleObject *libraryClass);
        const std::vector<ModuleObject *> &GetLibraries() const;

    private:
        std::vector<ModuleObject *> mLibraries;
    };
}