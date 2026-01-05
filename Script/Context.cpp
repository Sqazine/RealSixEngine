#include "Context.hpp"
#include "LibraryManager.hpp"
#include "Allocator.hpp"
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
            Allocator::GetInstance().SetGlobalValue(i, lib);
        }
    }
    void Context::CleanUp()
    {
        LibraryManager::GetInstance().CleanUp();
        Allocator::GetInstance().CleanUp();
    }
}