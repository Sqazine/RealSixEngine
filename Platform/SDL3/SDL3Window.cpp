#include "SDL3Window.hpp"
#include "Platform/PlatformInfo.hpp"
#include "SDL3/SDL_vulkan.h"
#include "Config/Config.hpp"
namespace RealSix
{
    SDL3Window::SDL3Window()
        : mHandle(nullptr), mIsShown(false), mTitle("RealSix")
    {
        auto defaultDisplayInfo = PlatformInfo::GetInstance().GetHardwareInfo()->GetDisplayInfos()[0];

        uint32_t windowFlag = SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
        const GfxConfig &gfxConfig = GfxConfig::GetInstance();
        switch (gfxConfig.GetBackend())
        {
        case GfxBackend::VULKAN:
            windowFlag |= SDL_WINDOW_VULKAN;
            break;
        case GfxBackend::METAL:
            windowFlag |= SDL_WINDOW_METAL;
            break;
        case GfxBackend::D3D12:
            REALSIX_LOG_INFO("D3D12 backend doesn't need Window flag in SDL3Window.");
            break;
        default:
            REALSIX_LOG_ERROR("Unsupported GfxBackend: {}", static_cast<int>(gfxConfig.GetBackend()));
            return;
        }

        auto aspect = 4.0f / 3.0f;
        auto ratio = 1.0f / aspect;

        int32_t actualWidth;
        int32_t actualHeight;

        if (defaultDisplayInfo.width > defaultDisplayInfo.height)
        {
            actualHeight = static_cast<int32_t>(defaultDisplayInfo.height * ratio);
            actualWidth = static_cast<int32_t>(actualHeight * aspect);
        }
        else
        {
            actualWidth = static_cast<int32_t>(defaultDisplayInfo.width * ratio);
            actualHeight = static_cast<int32_t>(actualWidth / aspect);
        }

        mHandle = SDL_CreateWindow(mTitle.CString(), actualWidth, actualHeight, windowFlag);
    }

    SDL3Window::~SDL3Window()
    {
        if (mHandle)
            SDL_DestroyWindow(mHandle);
    }

    void SDL3Window::SetTitle(StringView str)
    {
        mTitle = str;
        SDL_SetWindowTitle(mHandle, mTitle.CString());
    }

    StringView SDL3Window::GetTitle() const
    {
        return SDL_GetWindowTitle(mHandle);
    }

    void SDL3Window::Resize(const Vector2u32 &extent)
    {
        Resize(extent.x, extent.y);
    }

    void SDL3Window::Resize(uint32_t w, uint32_t h)
    {
        SDL_SetWindowSize(mHandle, w, h);
        SDL_SetWindowPosition(mHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SetEvent(Event::RESIZE);
    }

    Vector2u32 SDL3Window::GetSize()
    {
        int32_t x, y;
        SDL_GetWindowSize(mHandle, &x, &y);
        return Vector2u32(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
    }

    float SDL3Window::GetAspect()
    {
        auto size = GetSize();
        return static_cast<float>(size.x) / static_cast<float>(size.y);
    }

    SDL_Window *SDL3Window::GetHandle() const
    {
        return mHandle;
    }

    void SDL3Window::Show()
    {
        if (!mIsShown)
        {
            SDL_ShowWindow(mHandle);
            mIsShown = true;
        }
    }
    void SDL3Window::Hide()
    {
        if (mIsShown)
        {
            SDL_HideWindow(mHandle);
            mIsShown = false;
        }
    }
}