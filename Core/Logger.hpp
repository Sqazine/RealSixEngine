#pragma once
#include <iostream>
#include <sstream>
#include "Core/Config.hpp"
#include "String.hpp"
#include <cassert>
#include <cstdarg>
#include "Core/Marco.hpp"
namespace RealSix::Logger
{
    enum class Kind
    {
        INFO,
        WARN,
        ERROR
    };

    inline void Output(std::ostream &os, String s)
    {
        os << s;
    }

    template <typename T, typename... Args>
    inline void Output(std::ostream &os, String s, const T &next, const Args &...args)
    {
        auto index = s.FindFirstOf("{}");
        if (s.IsAtLast(index))
            Output(os, s);
        else
        {
            std::stringstream sstr;
            sstr << next;
            s.Replace(index, 2, sstr.str());
            sstr.clear();
            Output(os, s, args...);
        }
    }

    template <typename... Args>
    inline void Println(const String &s, const Args &...args)
    {
        Output(std::cout, s + "\n", args...);
    }

    template <typename... Args>
    inline void Print(const String &s, const Args &...args)
    {
        Output(std::cout, s, args...);
    }

    template <typename... Args>
    void Log(Kind logKind, const String &fmt, const Args &...args)
    {
        switch (logKind)
        {
        case Kind::ERROR:
            Println("\033[31m[ERROR]:" + fmt + "\033[0m", args...);
            break;
        case Kind::WARN:
            Println("\033[33m[WARN]:" + fmt + "\033[0m", args...);
            break;
        case Kind::INFO:
            Println("\033[32m[INFO]:" + fmt + "\033[0m", args...);
            break;
        default:
            break;
        }
    }

#define REALSIX_LOG_ERROR(fmt, ...)                                             \
    do                                                                          \
    {                                                                           \
        RealSix::Logger::Log(RealSix::Logger::Kind::ERROR, fmt, ##__VA_ARGS__); \
        abort();                                                                \
    } while (false)

#define REALSIX_LOG_WARN(fmt, ...)                                                  \
    do                                                                              \
    {                                                                               \
        if (RealSix::LoggerConfig::GetInstance().IsWarnningAsError())               \
            RealSix::Logger::Log(RealSix::Logger::Kind::ERROR, fmt, ##__VA_ARGS__); \
        else                                                                        \
            RealSix::Logger::Log(RealSix::Logger::Kind::WARN, fmt, ##__VA_ARGS__);  \
    } while (false)

#define REALSIX_LOG_INFO(fmt, ...)                                             \
    do                                                                         \
    {                                                                          \
        RealSix::Logger::Log(RealSix::Logger::Kind::INFO, fmt, ##__VA_ARGS__); \
    } while (false)
}