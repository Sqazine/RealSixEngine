#pragma once
#include <iostream>
#include <vector>
#include "Chunk.h"
#include "Object.h"
#include "LibraryManager.h"
namespace RealSix::Script
{
    class REALSIX_API VM
    {
    public:
        constexpr VM() noexcept = default;
        constexpr ~VM() noexcept = default;

        std::vector<Value> Run(FunctionObject *mainFunc) noexcept;

    private:
        void Execute();

        bool IsFalsey(const Value &v) noexcept;

        void CreateParentClassInstance(ClassInstanceObject* classInstance);
    };
}