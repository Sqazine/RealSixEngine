#pragma once
#include <memory>
#include <string_view>
#include <string>
#include <vector>
#include <array>
#include "Core/Marco.h"

#define STACK_MAX 1024
#define GLOBAL_VARIABLE_MAX 512

#define UINT8_COUNT (UINT8_MAX + 1)

#define GC_HEAP_GROW_FACTOR 2

#define MAIN_ENTRY_FUNCTION_NAME TEXT("_main_start_up")

namespace RealSix::Script
{
    enum class Permission : uint8_t
    {
        MUTABLE,
        IMMUTABLE,
    };

    enum class VarArg : uint8_t
    {
        NONE = 0,
        WITHOUT_NAME,
        WITH_NAME,
    };

    STRING PointerAddressToString(void *pointer);

    int64_t Factorial(int64_t v, int64_t tmp = 1);

    uint64_t NormalizeIdx(int64_t idx, size_t dsSize);

    uint32_t HashString(CHAR_T *str);

    namespace ByteConverter
    {
        std::array<uint8_t, 8> ToU64ByteList(int64_t integer);
        uint64_t GetU64Integer(const std::vector<uint8_t> data, uint32_t start);

        std::array<uint8_t, 4> ToU32ByteList(int32_t integer);
        uint32_t GetU32Integer(const std::vector<uint8_t> data, uint32_t start);
    }
}