#include "LibraryManager.hpp"
#include <stdio.h>
#include <ctime>
#include <iostream>
#include "Common.hpp"
#include "Logger.hpp"
#include "Allocator.hpp"

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
        const auto SizeOfFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                             {
                                                                 if (args == nullptr || argCount > 1)
                                                                     REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'sizeof']:Expect a argument.");

                                                                 if (IS_ARRAY_VALUE(args[0]))
                                                                 {
                                                                     result = Value((int64_t)TO_ARRAY_VALUE(args[0])->elements.size());
                                                                     return true;
                                                                 }
                                                                 else if (IS_DICT_VALUE(args[0]))
                                                                 {
                                                                     result = Value((int64_t)TO_DICT_VALUE(args[0])->elements.size());
                                                                     return true;
                                                                 }
                                                                 else if (IS_STR_VALUE(args[0]))
                                                                 {
                                                                     result = Value((int64_t)TO_STR_VALUE(args[0])->value.Size());
                                                                     return true;
                                                                 }
                                                                 else
                                                                     REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'sizeof']:Expect a array,dict ot string argument.");

                                                                 return false;
                                                             });

        const auto InsertFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                             {
                                                                 if (args == nullptr || argCount != 3)
                                                                     REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Expect 3 arguments,the arg0 must be array,dict or string object.The arg1 is the index object.The arg2 is the value object.");

                                                                 if (IS_ARRAY_VALUE(args[0]))
                                                                 {
                                                                     ArrayObject *array = TO_ARRAY_VALUE(args[0]);
                                                                     if (!IS_INT_VALUE(args[1]))
                                                                         REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Arg1 must be integer type while insert to a array");

                                                                     int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                     if (iIndex < 0 || iIndex >= (int64_t)array->elements.size())
                                                                         REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Index out of array's range");

                                                                     array->elements.insert(array->elements.begin() + iIndex, 1, args[2]);
                                                                 }
                                                                 else if (IS_DICT_VALUE(args[0]))
                                                                 {
                                                                     DictObject *dict = TO_DICT_VALUE(args[0]);

                                                                     for (auto [key, value] : dict->elements)
                                                                         if (key == args[1])
                                                                             REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Already exist value in the dict object of arg1" + args[1].ToString());

                                                                     dict->elements[args[1]] = args[2];
                                                                 }
                                                                 else if (IS_STR_VALUE(args[0]))
                                                                 {
                                                                     auto &string = TO_STR_VALUE(args[0])->value;
                                                                     if (!IS_INT_VALUE(args[1]))
                                                                         REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Arg1 must be integer type while insert to a array");

                                                                     int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                     if (iIndex < 0 || iIndex >= (int64_t)string.Size())
                                                                         REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Index out of array's range");

                                                                     string.Insert(iIndex, args[2].ToString());
                                                                 }
                                                                 else
                                                                     REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'insert']:Expect a array,dict ot string argument.");

                                                                 result = args[0];
                                                                 return true;
                                                             });

        const auto EraseFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                            {
                                                                if (args == nullptr || argCount != 2)
                                                                    REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:Expect 2 arguments,the arg0 must be array,dict or string object.The arg1 is the corresponding index object.");

                                                                if (IS_ARRAY_VALUE(args[0]))
                                                                {
                                                                    ArrayObject *array = TO_ARRAY_VALUE(args[0]);
                                                                    if (!IS_INT_VALUE(args[1]))
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:Arg1 must be integer type while insert to a array");

                                                                    int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                    if (iIndex < 0 || iIndex >= (int64_t)array->elements.size())
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:Index out of array's range");

                                                                    array->elements.erase(array->elements.begin() + iIndex);
                                                                }
                                                                else if (IS_DICT_VALUE(args[0]))
                                                                {
                                                                    DictObject *dict = TO_DICT_VALUE(args[0]);

                                                                    bool hasValue = false;

                                                                    for (auto it = dict->elements.begin(); it != dict->elements.end(); ++it)
                                                                        if (it->first == args[1])
                                                                        {
                                                                            dict->elements.erase(it);
                                                                            hasValue = true;
                                                                            break;
                                                                        }

                                                                    if (!hasValue)
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:No corresponding index in dict.");
                                                                }
                                                                else if (IS_STR_VALUE(args[0]))
                                                                {
                                                                    String &string = TO_STR_VALUE(args[0])->value;
                                                                    if (!IS_INT_VALUE(args[1]))
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:Arg1 must be integer type while insert to a array");

                                                                    int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                    if (iIndex < 0 || iIndex >= (int64_t)string.Size())
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:Index out of array's range");

                                                                    string.Erase(iIndex);
                                                                }
                                                                else
                                                                    REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'erase']:Expect a array,dict ot string argument.");

                                                                result = args[0];
                                                                return true;
                                                            });

        const auto AddressOfFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                                {
                                                                    if (args == nullptr || argCount != 1)
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'addressof']:Expect 1 arguments.");

                                                                    if (!IS_OBJECT_VALUE(args[0]))
                                                                        REALSIX_SCRIPT_LOG_ERROR(relatedToken, "[Native function 'addressof']:The arg0 is a value,only object has address.");

                                                                    result = new StrObject(PointerAddressToString(args[0].object));
                                                                    return true;
                                                                });

        const auto ClockFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                            {
                                                                result = Value((double)clock() / CLOCKS_PER_SEC);
                                                                return true;
                                                            });

        auto ioModule = new ModuleObject("io");
        auto dsModule = new ModuleObject("ds");
        auto memModule = new ModuleObject("mem");
        auto timeModule = new ModuleObject("time");

        ioModule->members["print"] = new NativeFunctionObject(PRINT_LAMBDA(RealSix::Logger::Print));
        ioModule->members["println"] = new NativeFunctionObject(PRINT_LAMBDA(RealSix::Logger::Println));

        dsModule->members["sizeof"] = SizeOfFunction;
        dsModule->members["insert"] = InsertFunction;
        dsModule->members["erase"] = EraseFunction;

        memModule->members["addressof"] = AddressOfFunction;

        timeModule->members["clock"] = ClockFunction;

        mLibraries.emplace_back(ioModule);
        mLibraries.emplace_back(dsModule);
        mLibraries.emplace_back(memModule);
        mLibraries.emplace_back(timeModule);
    }

    void LibraryManager::Destroy()
    {
        mLibraries.clear();
    }
}