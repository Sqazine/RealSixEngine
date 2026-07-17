#pragma once
#include "Object.hpp"

namespace RealSix::Script
{
    class REALSIX_API TimeLibrary : public ModuleObject
    {
    public:
        TimeLibrary()
            : ModuleObject("time")
        {
            const auto ClockFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                                {
                                                                    result = Value((double)clock() / CLOCKS_PER_SEC);
                                                                    return true;
                                                                });

            members["clock"] = ClockFunction;
        }
    };
}