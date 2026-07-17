#pragma once

namespace RealSix::Script
{
    class REALSIX_API MemLibrary : public ModuleObject
    {
    public:
        MemLibrary()
            : ModuleObject("mem")
        {
            const auto AddressOfFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                                    {
                                                                        if (args == nullptr || argCount != 1)
                                                                            REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'addressof']:Expect 1 arguments.");

                                                                        if (!IS_OBJECT_VALUE(args[0]))
                                                                            REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'addressof']:The arg0 is a value,only object has address.");

                                                                        result = new StrObject(PointerAddressToString(args[0].object));
                                                                        return true;
                                                                    });

            members["addressof"] = AddressOfFunction;
        }
    };
}