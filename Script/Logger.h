#pragma once
#include <iostream>
#include <sstream>
#include <string_view>
#include <string>
#include <cassert>
#include <cstdarg>
#include "Token.h"
#include "Utils.h"
#include "Core/Marco.h"
#include "Logger/Logger.h"

namespace RealSix::Script
{
    namespace Record
    {
        inline STRING mCurFilePath = TEXT("repl");
        inline STRING mSourceCode = TEXT("");
    }

    inline void RecordSource(STRING_VIEW sourceCode)
    {
        Record::mSourceCode = sourceCode;
    }

    template <typename... Args>
    inline void AssemblyLogInfo(const STRING &headerHint, const STRING &colorHint, uint64_t lineNum, uint64_t column, uint64_t pos, const STRING &fmt, const Args &...args)
    {
        auto start = pos;
        auto end = pos;

        if (Record::mCurFilePath != TEXT("interpreter"))
        {
            while ((Record::mSourceCode[start - 1] != TCHAR('\n') && Record::mSourceCode[start - 1] != TCHAR('\r')) && start - 1 > 0)
                start--;

            while ((Record::mSourceCode[end] != TCHAR('\n') && Record::mSourceCode[end] != TCHAR('\r')) && end < Record::mSourceCode.size())
                end++;
        }
        else
        {
            start = 0;
            end = Record::mSourceCode.size();
        }

        auto startStr = headerHint + TEXT(":") + Record::mCurFilePath + TEXT("(line ") + TO_STRING(lineNum) + TEXT(",column ") + TO_STRING(column) + TEXT("): ");

        auto lineSrcCode = Record::mSourceCode.substr(start, end - start);

        RealSix::Logger::Println(TEXT("\033[{}m{}{}\033[0m"), colorHint, startStr, lineSrcCode);

        auto blankSize = startStr.size() + pos - start;

        STRING errorHintStr;
        errorHintStr.insert(0, blankSize, TCHAR(' '));
        errorHintStr += TEXT("^ ") + STRING(fmt);

        Logger::Println(TEXT("\033[{}m") + errorHintStr + TEXT("\033[0m"), colorHint, args...);
    }

    template <typename... Args>
    void Log(Logger::Kind logKind, int32_t pos, const STRING &fmt, const Args &...args)
    {
        auto lineNum = 1;
        for (int32_t i = 0; i < pos; ++i)
            if (Record::mSourceCode[i] == TCHAR('\n') || Record::mSourceCode[i] == TCHAR('\r'))
                lineNum++;

        switch (logKind)
        {
        case Logger::Kind::INFO:
            AssemblyLogInfo(TEXT("[INFO]"), TEXT("32"), lineNum, 1, pos, fmt, args...);
            break;
        case Logger::Kind::WARN:
            AssemblyLogInfo(TEXT("[WARN]"), TEXT("33"), lineNum, 1, pos, fmt, args...);
            break;
        case Logger::Kind::ERROR:
            AssemblyLogInfo(TEXT("[ERROR]"), TEXT("31"), lineNum, 1, pos, fmt, args...);
            break;
        default:
            break;
        }
    }

    template <typename... Args>
    void Log(Logger::Kind logKind, const Token *tok, const STRING &fmt, const Args &...args)
    {
        switch (logKind)
        {
        case Logger::Kind::INFO:
            AssemblyLogInfo(TEXT("[INFO]"), TEXT("32"), tok->sourceLocation.line, tok->sourceLocation.column, tok->sourceLocation.pos, fmt, args...);
            break;
        case Logger::Kind::WARN:
            AssemblyLogInfo(TEXT("[WARN]"), TEXT("33"), tok->sourceLocation.line, tok->sourceLocation.column, tok->sourceLocation.pos, fmt, args...);
            break;
        case Logger::Kind::ERROR:
            AssemblyLogInfo(TEXT("[ERROR]"), TEXT("31"), tok->sourceLocation.line, tok->sourceLocation.column, tok->sourceLocation.pos, fmt, args...);
            break;
        default:
            break;
        }
    }

#define REALSIX_LOG_ERROR_WITH_LOC(tokOrPos, fmt, ...)                           \
    do                                                                           \
    {                                                                            \
        RealSix::Script::Log(Logger::Kind::ERROR, tokOrPos, fmt, ##__VA_ARGS__); \
        assert(0);                                                               \
    } while (false)

#define REALSIX_LOG_WARN_WITH_LOC(tokOrPos, fmt, ...)                           \
    do                                                                          \
    {                                                                           \
        RealSix::Script::Log(Logger::Kind::WARN, tokOrPos, fmt, ##__VA_ARGS__); \
    } while (false)

#define REALSIX_LOG_INFO_WITH_LOC(tokOrPos, fmt, ...)                           \
    do                                                                          \
    {                                                                           \
        RealSix::Script::Log(Logger::Kind::INFO, tokOrPos, fmt, ##__VA_ARGS__); \
    } while (false)

}