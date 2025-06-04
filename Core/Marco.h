#ifndef REALSIX_BUILD_STATIC
#if defined(_WIN32) || defined(_WIN64)
#ifdef REALSIX_BUILD_DLL
#define REALSIX_API __declspec(dllexport)
#else
#define REALSIX_API __declspec(dllimport)
#endif
#else
#ifdef REALSIX_BUILD_DLL
#define REALSIX_API
#else
#define REALSIX_API
#endif
#endif
#else
#define REALSIX_API
#endif

#ifdef REALSIX_UTF8_ENCODE
#define STR(x) L##x
#define TEXT(x) STR(x)
#define TCHAR(x) STR(x)
#define CHAR_T wchar_t
#define STRING std::wstring
#define STRING_VIEW std::wstring_view
#define OSTREAM std::wostream
#define STRING_STREAM std::wstringstream
#define STD_IFSTREAM std::wifstream
#define TO_STRING(x) std::to_wstring(x)
#define COUT std::wcout
#define CIN std::wcin
#define STRCMP wcscmp
#define STRLEN wcslen
#else
#define STR(x) x
#define TEXT(x) STR(x)
#define TCHAR(x) STR(x)
#define CHAR_T char
#define STRING std::string
#define STRING_VIEW std::string_view
#define OSTREAM std::ostream
#define STRING_STREAM std::stringstream
#define STD_IFSTREAM std::ifstream
#define TO_STRING(x) std::to_string(x)
#define COUT std::cout
#define CIN std::cin
#define STRCMP strcmp
#define STRLEN strlen
#endif

#define SAFE_DELETE(x)   \
    do                   \
    {                    \
        if (x)           \
        {                \
            delete x;    \
            x = nullptr; \
        }                \
    } while (false);

#define SAFE_DELETE_ARRAY(x) \
    do                       \
    {                        \
        if (x)               \
        {                    \
            delete[] x;      \
            x = nullptr;     \
        }                    \
    } while (false);

#ifndef NDEBUG
#define REALSIX_CHECK(expr, msg)    \
    do                              \
    {                               \
        if (!(expr))                \
        {                           \
            REALSIX_LOG_ERROR(msg); \
        }                           \
    } while (false);
#else
#define REALSIX_CHECK(expr, msg)
#endif

#define ENUM_CLASS_OP(CastName, EnumName)                            \
    inline EnumName operator|(EnumName lhs, EnumName rhs)            \
    {                                                                \
        return static_cast<EnumName>(CastName(lhs) | CastName(rhs)); \
    }                                                                \
    inline EnumName operator&(EnumName lhs, EnumName rhs)            \
    {                                                                \
        return static_cast<EnumName>(CastName(lhs) & CastName(rhs)); \
    }