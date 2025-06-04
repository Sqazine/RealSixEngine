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

#define SAFE_DELETE(x)   \
    do                   \
    {                    \
        if (x)           \
        {                \
            delete x;    \
            x = nullptr; \
        }                \
    } while (false)

#define SAFE_DELETE_ARRAY(x) \
    do                       \
    {                        \
        if (x)               \
        {                    \
            delete[] x;      \
            x = nullptr;     \
        }                    \
    } while (false)

#ifndef NDEBUG
#define REALSIX_CHECK(expr, msg)    \
    do                              \
    {                               \
        if (!(expr))                \
        {                           \
            REALSIX_LOG_ERROR(msg); \
        }                           \
    } while (false)
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