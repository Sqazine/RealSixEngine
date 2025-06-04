#pragma once
#include <unordered_map>
#include <functional>
#include "Value.h"
#include "Object.h"
#include "Common.h"
namespace RealSix::Script
{
    class REALSIX_API LibraryManager:public Singleton<LibraryManager>
    {
    public:
        void Init();
        void Destroy();

        void RegisterLibrary(ModuleObject *libraryClass);
        const std::vector<ModuleObject *> &GetLibraries() const;

    private:
        std::vector<ModuleObject *> mLibraries;
    };
}