#pragma once
#include "Core/Common.hpp"
#include "Core/Marco.hpp"
namespace RealSix
{
    class AppConfig : public Singleton<AppConfig>
    {
    public:
        AppConfig &SetRefreshOnlyWindowIsActive(bool isActive);
        bool IsRefreshOnlyWindowIsActive();

    private:
        bool mRefreshOnlyWindowIsActive{true};
    };

    enum class GfxBackend: uint8_t
    {
        VULKAN = 0,
        D3D12 = 1,
        METAL = 2,
    };

    enum class GfxBackBufferCount : uint8_t
    {
        ONE = 1,
        TWO = 2,
        THREE = 3,

        MAX = THREE,
    };

    enum class Msaa : uint8_t
    {
        X1  = 1,
        X2  = 2,
        X4  = 4,
        X8  = 8,
        X16 = 16,
        X32 = 32,
        X64 = 64,

        MAX = X64
    };

    class GfxConfig : public Singleton<GfxConfig>
    {
    public:
        GfxConfig &SetBackend(GfxBackend backend);
        GfxBackend GetBackend() const;
        GfxConfig &SetVSync(bool vSync);
        bool GetVSync() const;
        GfxConfig &SetBackBufferCount(GfxBackBufferCount count);
        GfxBackBufferCount GetBackBufferCount() const;
        GfxConfig &SetMsaa(Msaa msaa);
        Msaa GetMsaa() const;

    private:
        GfxBackend mBackend{GfxBackend::VULKAN};
        bool mVSync{false};
        GfxBackBufferCount mBackBufferCount{GfxBackBufferCount::TWO};
        Msaa mMsaa{Msaa::X1};

#ifndef NDEBUG
    public:
        GfxConfig &SetEnableGpuValidation(bool use);
        bool IsEnableGpuValidation() const;

    private:
        bool mEnableGpuValidation{true};
#endif
    };

    class REALSIX_API ScriptConfig : public Singleton<ScriptConfig>
    {
    public:
        ScriptConfig &SetExecuteFilePath(StringView path);
        const String &GetExecuteFilePath() const;

        ScriptConfig &SetUseFunctionCache(bool toggle);
        bool IsUseFunctionCache() const;

        ScriptConfig &SetSerializeBinaryChunk(bool toggle);
        bool IsSerializeBinaryChunk() const;

        ScriptConfig &SetSerializeBinaryFilePath(StringView path);
        StringView GetSerializeBinaryFilePath() const;

        String ToFullPath(StringView filePath);

    private:
        String mExecuteFilePath;

        bool mUseFunctionCache{false};

        bool mIsSerializeBinaryChunk{false};
        StringView mSerializeBinaryFilePath;

#ifndef NDEBUG
    public:
        ScriptConfig &SetDebugGC(bool toggle);
        bool IsDebugGC() const;

        ScriptConfig &SetStressGC(bool toggle);
        bool IsStressGC() const;

    private:
        bool mDebugGC{false};
        bool mStressGC{false};
#endif
    };
}