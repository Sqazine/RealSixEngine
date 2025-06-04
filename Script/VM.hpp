#pragma once
#include <iostream>
#include <vector>
#include "Chunk.hpp"
#include "Object.hpp"
#include "LibraryManager.hpp"
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