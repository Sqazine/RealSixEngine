#include "Common.hpp"
#include <filesystem>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>
#include "Allocator.hpp"
#include "LibraryManager.hpp"
#include "Resource/IO.hpp"
#include "Core/Logger.hpp"
#include "Core/Marco.hpp"

namespace RealSix::Script
{
    String PointerAddressToString(void *pointer)
    {
        std::stringstream sstr;
        sstr << pointer;
        String address = sstr.str();
        return address;
    }

    int64_t Factorial(int64_t v, int64_t tmp)
    {
        if (v == 0)
            return tmp;
        return Factorial(v - 1, v * tmp);
    }

    uint64_t NormalizeIdx(int64_t idx, size_t dsSize)
    {
        bool isNeg = false;
        if (idx < 0)
        {
            isNeg = true;
            idx = abs(idx);
        }

        idx = idx % (int64_t)dsSize;

        if (isNeg)
            idx = dsSize - idx;

        return idx;
    }

    namespace ByteConverter
    {
        std::array<uint8_t, 8> ToU64ByteList(int64_t integer)
        {
            std::array<uint8_t, 8> result{};
            result[0] = (uint8_t((integer & 0xFF00000000000000) >> 56));
            result[1] = (uint8_t((integer & 0x00FF000000000000) >> 48));
            result[2] = (uint8_t((integer & 0x0000FF0000000000) >> 40));
            result[3] = (uint8_t((integer & 0x000000FF00000000) >> 32));
            result[4] = (uint8_t((integer & 0x00000000FF000000) >> 24));
            result[5] = (uint8_t((integer & 0x0000000000FF0000) >> 16));
            result[6] = (uint8_t((integer & 0x000000000000FF00) >> 8));
            result[7] = (uint8_t((integer & 0x00000000000000FF) >> 0));
            return result;
        }

        uint64_t GetU64Integer(const std::vector<uint8_t> data, uint32_t start)
        {
            uint64_t v{0};
            for (int32_t i = 0; i < 8; ++i)
                v |= ((uint64_t)(data[start + i] & 0x00000000000000FF) << ((7 - i) * 8));
            return v;
        }

        std::array<uint8_t, 4> ToU32ByteList(int32_t integer)
        {
            std::array<uint8_t, 4> result{};
            result[0] = (uint8_t((integer & 0xFF000000) >> 24));
            result[1] = (uint8_t((integer & 0x00FF0000) >> 16));
            result[2] = (uint8_t((integer & 0x0000FF00) >> 8));
            result[3] = (uint8_t((integer & 0x000000FF) >> 0));
            return result;
        }

        uint32_t GetU32Integer(const std::vector<uint8_t> data, uint32_t start)
        {
            uint32_t v{0};
            for (int32_t i = 0; i < 4; ++i)
                v |= ((uint32_t)(data[start + i] & 0x000000FF) << ((3 - i) * 8));
            return v;
        }
    }

    
}