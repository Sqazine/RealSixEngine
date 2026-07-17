#pragma once
#include "Object.hpp"

namespace RealSix::Script
{

#define PRINT_LAMBDA(fn) [](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool \
{                                                                                                             \
    if (args == nullptr)                                                                                      \
        return false;                                                                                         \
    if (argCount == 1)                                                                                        \
    {                                                                                                         \
        fn("{}", args[0].ToString());                                                                         \
        return false;                                                                                         \
    }                                                                                                         \
    if (!IS_STR_VALUE(args[0]))                                                                               \
    {                                                                                                         \
        for (uint32_t i = 0; i < argCount; ++i)                                                               \
            fn("{}", args[i].ToString());                                                                     \
        return false;                                                                                         \
    }                                                                                                         \
    String content = TO_STR_VALUE(args[0])->value;                                                            \
    if (argCount != 1) /*formatting output*/                                                                  \
    {                                                                                                         \
        size_t pos = content.Find("{}");                                                                      \
        size_t argpos = 1;                                                                                    \
        while (!content.IsAtLast(pos))                                                                        \
        {                                                                                                     \
            if (argpos < argCount)                                                                            \
                content.Replace(pos, 2, args[argpos++].ToString());                                           \
            else                                                                                              \
                content.Replace(pos, 2, "null");                                                              \
            pos = content.Find("{}");                                                                         \
        }                                                                                                     \
    }                                                                                                         \
    size_t pos = content.Find("\\n");                                                                         \
    while (!content.IsAtLast(pos))                                                                            \
    {                                                                                                         \
        content[pos] = '\n';                                                                                  \
        content.Replace(pos + 1, 1, ""); /*erase a char*/                                                     \
        pos = content.Find("\\n");                                                                            \
    }                                                                                                         \
    pos = content.Find("\\t");                                                                                \
    while (!content.IsAtLast(pos))                                                                            \
    {                                                                                                         \
        content[pos] = '\t';                                                                                  \
        content.Replace(pos + 1, 1, ""); /*erase a char*/                                                     \
        pos = content.Find("\\t");                                                                            \
    }                                                                                                         \
    pos = content.Find("\\r");                                                                                \
    while (!content.IsAtLast(pos))                                                                            \
    {                                                                                                         \
        content[pos] = '\r';                                                                                  \
        content.Replace(pos + 1, 1, ""); /*erase a char*/                                                     \
        pos = content.Find("\\r");                                                                            \
    }                                                                                                         \
    fn("{}", content);                                                                                        \
    return false;                                                                                             \
}

    class REALSIX_API IOLibrary : public ModuleObject
    {
    public:
        IOLibrary()
            : ModuleObject("io")
        {
            members["print"] = new NativeFunctionObject(PRINT_LAMBDA(RealSix::Logger::Print));
            members["println"] = new NativeFunctionObject(PRINT_LAMBDA(RealSix::Logger::Println));
        }
    };
}