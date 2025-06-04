#pragma once
#include <iostream>
#include <sstream>

#include "String.hpp"
#include <cassert>
#include <cstdarg>
#include "Token.hpp"
#include "Utils.hpp"
#include "Core/Marco.hpp"
#include "Core/Logger.hpp"

namespace RealSix::Script
{
    template <typename... Args>
    inline void AssemblyLogInfo(const String &headerHint, const String &colorHint, const SourceLocation &sourceLocation, const String &fmt, const Args &...args)
    {
        auto start = sourceLocation.pos;
        auto end = sourceLocation.pos;

        if (sourceLocation.filePath != "interpreter")
        {
            while ((sourceLocation.sourceCode[start - 1] != '\n') && sourceLocation.sourceCode[start - 1] != '\r' && start - 1 > 0)
                start--;

            while ((sourceLocation.sourceCode[end] != '\n') && sourceLocation.sourceCode[end] != '\r' && end < sourceLocation.sourceCode.Size())
                end++;
        }
        else
        {
            start = 0;
            end = sourceLocation.sourceCode.Size();
        }

        auto startStr = headerHint + ":" + sourceLocation.filePath + "(line " + std::to_string(sourceLocation.line) + ",column " + std::to_string(sourceLocation.column) + "): ";

        auto lineSrcCode = sourceLocation.sourceCode.SubStr(start, end - start);

        RealSix::Logger::Println("\033[{}m{}{}\033[0m", colorHint, startStr, lineSrcCode);

        auto blankSize = startStr.Size() + sourceLocation.pos - start;

        String errorHintStr;
        errorHintStr.Insert(0, blankSize, ' ');
        errorHintStr += "^ " + String(fmt);

        Logger::Println("\033[{}m" + errorHintStr + "\033[0m", colorHint, args...);
    }

    template <typename... Args>
    void Log(Logger::Kind logKind, const SourceLocation &sourceLocation, const String &fmt, const Args &...args)
    {
        auto lineNum = 1;
        for (int32_t i = 0; i < sourceLocation.pos; ++i)
            if (sourceLocation.sourceCode[i] == '\n' || sourceLocation.sourceCode[i] == '\r')
                lineNum++;

        switch (logKind)
        {
        case Logger::Kind::INFO:
            AssemblyLogInfo("[INFO]", "32", sourceLocation, fmt, args...);
            break;
        case Logger::Kind::WARN:
            AssemblyLogInfo("[WARN]", "33", sourceLocation, fmt, args...);
            break;
        case Logger::Kind::ERROR:
            AssemblyLogInfo("[ERROR]", "31", sourceLocation, fmt, args...);
            break;
        default:
            break;
        }
    }

    template <typename... Args>
    void Log(Logger::Kind logKind, const Token *tok, const String &fmt, const Args &...args)
    {
        switch (logKind)
        {
        case Logger::Kind::INFO:
            AssemblyLogInfo("[INFO]", "32", tok->sourceLocation, fmt, args...);
            break;
        case Logger::Kind::WARN:
            AssemblyLogInfo("[WARN]", "33", tok->sourceLocation, fmt, args...);
            break;
        case Logger::Kind::ERROR:
            AssemblyLogInfo("[ERROR]", "31", tok->sourceLocation, fmt, args...);
            break;
        default:
            break;
        }
    }

#define REALSIX_SCRIPT_LOG_ERROR(tokOrSourceLocation, fmt, ...)                             \
    do                                                                                      \
    {                                                                                       \
        RealSix::Script::Log(Logger::Kind::ERROR, tokOrSourceLocation, fmt, ##__VA_ARGS__); \
        assert(0);                                                                          \
    } while (false)

#define REALSIX_SCRIPT_LOG_WARN(tokOrSourceLocation, fmt, ...)                             \
    do                                                                                     \
    {                                                                                      \
        RealSix::Script::Log(Logger::Kind::WARN, tokOrSourceLocation, fmt, ##__VA_ARGS__); \
    } while (false)

#define REALSIX_SCRIPT_LOG_INFO(tokOrSourceLocation, fmt, ...)                             \
    do                                                                                     \
    {                                                                                      \
        RealSix::Script::Log(Logger::Kind::INFO, tokOrSourceLocation, fmt, ##__VA_ARGS__); \
    } while (false)

}