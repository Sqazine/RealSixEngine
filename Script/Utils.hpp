#pragma once
#include <memory>

#include "String.hpp"
#include <vector>
#include <array>
#include "Core/Marco.hpp"

#define STACK_MAX 1024
#define VARIABLE_MAX 512
#define STATIC_VARIABLE_MAX 64

#define UINT8_COUNT (UINT8_MAX + 1)

#define GC_HEAP_GROW_FACTOR 2

#define MAIN_ENTRY_FUNCTION_NAME "_main_start_up"

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

    String PointerAddressToString(void *pointer);

    int64_t Factorial(int64_t v, int64_t tmp = 1);

    uint64_t NormalizeIdx(int64_t idx, size_t dsSize);

    namespace ByteConverter
    {
        std::array<uint8_t, 8> ToU64ByteList(int64_t integer);
        uint64_t GetU64Integer(const std::vector<uint8_t> data, uint32_t start);

        std::array<uint8_t, 4> ToU32ByteList(int32_t integer);
        uint32_t GetU32Integer(const std::vector<uint8_t> data, uint32_t start);
    }
}