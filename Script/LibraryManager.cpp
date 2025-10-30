#include "LibraryManager.h"
#include <stdio.h>
#include <ctime>
#include <iostream>
#include "Common.h"
#include "Logger.h"
#include "Allocator.h"

#define PRINT_LAMBDA(fn) [](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool \
{                                                                                                             \
    if (args == nullptr)                                                                                      \
        return false;                                                                                         \
    if (argCount == 1)                                                                                        \
    {                                                                                                         \
        fn(TEXT("{}"), args[0].ToString());                                                                   \
        return false;                                                                                         \
    }                                                                                                         \
    if (!IS_STR_VALUE(args[0]))                                                                           \
    {                                                                                                         \
        for (uint32_t i = 0; i < argCount; ++i)                                                               \
            fn(TEXT("{}"), args[i].ToString());                                                               \
        return false;                                                                                         \
    }                                                                                                         \
    STRING content = TO_STR_VALUE(args[0])->value;                                                        \
    if (argCount != 1) /*formatting output*/                                                                  \
    {                                                                                                         \
        size_t pos = content.find(TEXT("{}"));                                                                \
        size_t argpos = 1;                                                                                    \
        while (pos != STRING::npos)                                                                           \
        {                                                                                                     \
            if (argpos < argCount)                                                                            \
                content.replace(pos, 2, args[argpos++].ToString());                                           \
            else                                                                                              \
                content.replace(pos, 2, TEXT("null"));                                                        \
            pos = content.find(TEXT("{}"));                                                                   \
        }                                                                                                     \
    }                                                                                                         \
    size_t pos = content.find(TEXT("\\n"));                                                                   \
    while (pos != STRING::npos)                                                                               \
    {                                                                                                         \
        content[pos] = TCHAR('\n');                                                                           \
        content.replace(pos + 1, 1, TEXT("")); /*erase a char*/                                               \
        pos = content.find(TEXT("\\n"));                                                                      \
    }                                                                                                         \
    pos = content.find(TEXT("\\t"));                                                                          \
    while (pos != STRING::npos)                                                                               \
    {                                                                                                         \
        content[pos] = TCHAR('\t');                                                                           \
        content.replace(pos + 1, 1, TEXT("")); /*erase a char*/                                               \
        pos = content.find(TEXT("\\t"));                                                                      \
    }                                                                                                         \
    pos = content.find(TEXT("\\r"));                                                                          \
    while (pos != STRING::npos)                                                                               \
    {                                                                                                         \
        content[pos] = TCHAR('\r');                                                                           \
        content.replace(pos + 1, 1, TEXT("")); /*erase a char*/                                               \
        pos = content.find(TEXT("\\r"));                                                                      \
    }                                                                                                         \
    fn(TEXT("{}"), content);                                                                                  \
    return false;                                                                                             \
}

namespace RealSix::Script
{
    void LibraryManager::RegisterLibrary(ModuleObject *libraryClass)
    {
        for (const auto &lib : mLibraries)
        {
            if (lib->name == libraryClass->name)
                REALSIX_LOG_ERROR(TEXT("Conflict library name {}"), libraryClass->name);
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
                                                                     REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'sizeof']:Expect a argument."));

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
                                                                     result = Value((int64_t)TO_STR_VALUE(args[0])->value.size());
                                                                     return true;
                                                                 }
                                                                 else
                                                                     REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'sizeof']:Expect a array,dict ot string argument."));

                                                                 return false;
                                                             });

        const auto InsertFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                             {
                                                                 if (args == nullptr || argCount != 3)
                                                                     REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Expect 3 arguments,the arg0 must be array,dict or string object.The arg1 is the index object.The arg2 is the value object."));

                                                                 if (IS_ARRAY_VALUE(args[0]))
                                                                 {
                                                                     ArrayObject *array = TO_ARRAY_VALUE(args[0]);
                                                                     if (!IS_INT_VALUE(args[1]))
                                                                         REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Arg1 must be integer type while insert to a array"));

                                                                     int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                     if (iIndex < 0 || iIndex >= (int64_t)array->elements.size())
                                                                         REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Index out of array's range"));

                                                                     array->elements.insert(array->elements.begin() + iIndex, 1, args[2]);
                                                                 }
                                                                 else if (IS_DICT_VALUE(args[0]))
                                                                 {
                                                                     DictObject *dict = TO_DICT_VALUE(args[0]);

                                                                     for (auto [key, value] : dict->elements)
                                                                         if (key == args[1])
                                                                             REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Already exist value in the dict object of arg1") + args[1].ToString());

                                                                     dict->elements[args[1]] = args[2];
                                                                 }
                                                                 else if (IS_STR_VALUE(args[0]))
                                                                 {
                                                                     auto &string = TO_STR_VALUE(args[0])->value;
                                                                     if (!IS_INT_VALUE(args[1]))
                                                                         REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Arg1 must be integer type while insert to a array"));

                                                                     int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                     if (iIndex < 0 || iIndex >= (int64_t)string.size())
                                                                         REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Index out of array's range"));

                                                                     string.insert(iIndex, args[2].ToString());
                                                                 }
                                                                 else
                                                                     REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'insert']:Expect a array,dict ot string argument."));

                                                                 result = args[0];
                                                                 return true;
                                                             });

        const auto EraseFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                            {
                                                                if (args == nullptr || argCount != 2)
                                                                    REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:Expect 2 arguments,the arg0 must be array,dict or string object.The arg1 is the corresponding index object."));

                                                                if (IS_ARRAY_VALUE(args[0]))
                                                                {
                                                                    ArrayObject *array = TO_ARRAY_VALUE(args[0]);
                                                                    if (!IS_INT_VALUE(args[1]))
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:Arg1 must be integer type while insert to a array"));

                                                                    int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                    if (iIndex < 0 || iIndex >= (int64_t)array->elements.size())
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:Index out of array's range"));

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
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:No corresponding index in dict."));
                                                                }
                                                                else if (IS_STR_VALUE(args[0]))
                                                                {
                                                                    auto &string = TO_STR_VALUE(args[0])->value;
                                                                    if (!IS_INT_VALUE(args[1]))
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:Arg1 must be integer type while insert to a array"));

                                                                    int64_t iIndex = TO_INT_VALUE(args[1]);

                                                                    if (iIndex < 0 || iIndex >= (int64_t)string.size())
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:Index out of array's range"));

                                                                    string.erase(string.begin() + iIndex);
                                                                }
                                                                else
                                                                    REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'erase']:Expect a array,dict ot string argument."));

                                                                result = args[0];
                                                                return true;
                                                            });

        const auto AddressOfFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                                {
                                                                    if (args == nullptr || argCount != 1)
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'addressof']:Expect 1 arguments."));

                                                                    if (!IS_OBJECT_VALUE(args[0]))
                                                                        REALSIX_LOG_ERROR_WITH_LOC(relatedToken, TEXT("[Native function 'addressof']:The arg0 is a value,only object has address."));

                                                                    result = new StrObject(PointerAddressToString(args[0].object));
                                                                    return true;
                                                                });

        const auto ClockFunction = new NativeFunctionObject([](Value *args, uint32_t argCount, const Token *relatedToken, Value &result) -> bool
                                                            {
                                                                result = Value((double)clock() / CLOCKS_PER_SEC);
                                                                return true;
                                                            });

        auto ioModule = new ModuleObject(TEXT("io"));
        auto dsModule = new ModuleObject(TEXT("ds"));
        auto memModule = new ModuleObject(TEXT("mem"));
        auto timeModule = new ModuleObject(TEXT("time"));

        ioModule->members[TEXT("print")] = new NativeFunctionObject(PRINT_LAMBDA(Logger::Print));
        ioModule->members[TEXT("println")] = new NativeFunctionObject(PRINT_LAMBDA(Logger::Println));

        dsModule->members[TEXT("sizeof")] = SizeOfFunction;
        dsModule->members[TEXT("insert")] = InsertFunction;
        dsModule->members[TEXT("erase")] = EraseFunction;

        memModule->members[TEXT("addressof")] = AddressOfFunction;

        timeModule->members[TEXT("clock")] = ClockFunction;

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