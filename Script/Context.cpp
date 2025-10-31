#include "Context.h"
#include "LibraryManager.h"
#include "Allocator.h"
namespace RealSix::Script
{
    void Context::Init()
    {
        Allocator::GetInstance().Init();
        LibraryManager::GetInstance().Init();

        // Register built-in libraries
        for (size_t i = 0; i < LibraryManager::GetInstance().GetLibraries().size(); ++i)
        {
            ModuleObject *lib = LibraryManager::GetInstance().GetLibraries()[i];
            Allocator::GetInstance().SetGlobalVariable(i, lib);
        }
    }
    void Context::Destroy()
    {
        LibraryManager::GetInstance().Destroy();
        Allocator::GetInstance().Destroy();
    }
}