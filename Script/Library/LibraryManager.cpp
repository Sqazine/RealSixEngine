#include "LibraryManager.hpp"
#include <stdio.h>
#include <ctime>
#include <iostream>
#include "Common.hpp"
#include "Logger.hpp"
#include "Allocator.hpp"
#include "IOLibrary.hpp"
#include "DSLibrary.hpp"
#include "MemLibrary.hpp"
#include "TimeLibrary.hpp"

namespace RealSix::Script
{
    void LibraryManager::RegisterLibrary(ModuleObject *libraryClass)
    {
        for (const auto &lib : mLibraries)
        {
            if (lib->name == libraryClass->name)
                REALSIX_LOG_ERROR("Conflict library name {}", libraryClass->name);
        }

        mLibraries.emplace_back(libraryClass);
    }

    const std::vector<ModuleObject *> &LibraryManager::GetLibraries() const
    {
        return mLibraries;
    }

    void LibraryManager::Init()
    {
        mLibraries.emplace_back(new IOLibrary());
        mLibraries.emplace_back(new DSLibrary());
        mLibraries.emplace_back(new MemLibrary());
        mLibraries.emplace_back(new TimeLibrary());
    }

    void LibraryManager::CleanUp()
    {
        for (auto &lib : mLibraries)
            SAFE_DELETE(lib);

        mLibraries.clear();
    }
}