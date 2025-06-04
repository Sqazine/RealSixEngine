#include "Config.hpp"
#include <filesystem>
namespace RealSix
{
    AppConfig &AppConfig::SetRefreshOnlyWindowIsActive(bool isActive)
    {
        mRefreshOnlyWindowIsActive = isActive;
        return *this;
    }

    bool AppConfig::IsRefreshOnlyWindowIsActive()
    {
        return mRefreshOnlyWindowIsActive;
    }

    GfxConfig &GfxConfig::SetBackend(GfxBackend backend)
    {
        mBackend = backend;
        return *this;
    }

    GfxBackend GfxConfig::GetBackend() const
    {
        return mBackend;
    }

    GfxConfig &GfxConfig::SetVSync(bool vSync)
    {
        mVSync = vSync;
        return *this;
    }

    bool GfxConfig::GetVSync() const
    {
        return mVSync;
    }

    GfxConfig &GfxConfig::SetBackBufferCount(GfxBackBufferCount count)
    {
        mBackBufferCount = count;
        return *this;
    }

    GfxBackBufferCount GfxConfig::GetBackBufferCount() const
    {
        return mBackBufferCount;
    }

    GfxConfig &GfxConfig::SetMsaa(Msaa msaa)
    {
        mMsaa = msaa;
        return *this;
    }

    Msaa GfxConfig::GetMsaa() const
    {
        return mMsaa;
    }

#ifndef NDEBUG

    GfxConfig &GfxConfig::SetEnableGpuValidation(bool use)
    {
        mEnableGpuValidation = use;
        return *this;
    }

    bool GfxConfig::IsEnableGpuValidation() const
    {
        return mEnableGpuValidation;
    }

#endif

    ScriptConfig &ScriptConfig::SetExecuteFilePath(StringView path)
    {
        mExecuteFilePath = path;
        return *this;
    }

    const String &ScriptConfig::GetExecuteFilePath() const
    {
        return mExecuteFilePath;
    }

    ScriptConfig &ScriptConfig::SetUseFunctionCache(bool toggle)
    {
        mUseFunctionCache = toggle;
        return *this;
    }

    bool ScriptConfig::IsUseFunctionCache() const
    {
        return mUseFunctionCache;
    }

    ScriptConfig &ScriptConfig::SetSerializeBinaryChunk(bool toggle)
    {
        mIsSerializeBinaryChunk = toggle;
        return *this;
    }

    bool ScriptConfig::IsSerializeBinaryChunk() const
    {
        return mIsSerializeBinaryChunk;
    }

    ScriptConfig &ScriptConfig::SetSerializeBinaryFilePath(StringView path)
    {
        mSerializeBinaryFilePath = path;
        return *this;
    }

    StringView ScriptConfig::GetSerializeBinaryFilePath() const
    {
        return mSerializeBinaryFilePath;
    }

    String ScriptConfig::ToFullPath(StringView filePath)
    {
        std::filesystem::path filesysPath = filePath.GetRawData();
        String fullPath = filesysPath.string();
        if (!filesysPath.is_absolute())
            fullPath = mExecuteFilePath + fullPath;
        return fullPath;
    }
#ifndef NDEBUG
    ScriptConfig &ScriptConfig::SetDebugGC(bool toggle)
    {
        mDebugGC = toggle;
        return *this;
    }
    bool ScriptConfig::IsDebugGC() const
    {
        return mDebugGC;
    }
    ScriptConfig &ScriptConfig::SetStressGC(bool toggle)
    {
        mStressGC = toggle;
        return *this;
    }
    bool ScriptConfig::IsStressGC() const
    {
        return mStressGC;
    }
#endif
}