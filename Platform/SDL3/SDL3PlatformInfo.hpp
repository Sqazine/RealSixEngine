#pragma once

#include "Platform/PlatformInfo.hpp"
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL.h>
#include "Core/Logger.hpp"
#include "SDL3Window.hpp"
#include "Platform/Window.hpp"
namespace RealSix
{
    class SDL3VulkanPlatformInfo : public VulkanPlatformInfo
    {
        std::vector<const char *> GetInstanceWindowExtensionList()
        {
            uint32_t extensionCount;

            auto rawExts = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
            std::vector<const char *> result(extensionCount);
            for (size_t i = 0; i < extensionCount; ++i)
            {
                result[i] = rawExts[i];
            }
            return result;
        }

        VkSurfaceKHR CreateSurface(const Window *window, VkInstance instance)
        {
            SDL_Window *sdlWindow = static_cast<const SDL3Window *>(window)->GetHandle();
            VkSurfaceKHR result;
            SDL_Vulkan_CreateSurface(sdlWindow, instance, nullptr, &result);
            return result;
        }
    };

    class SDL3HardwareInfo : public HardwareInfo
    {
    public:
        void ObtainDisplayInfo() override
        {
            int numDisplays = 0;
            auto displays = SDL_GetDisplays(&numDisplays);

            if (numDisplays <= 0)
            {
                REALSIX_LOG_ERROR("Failed to get displays: {}", SDL_GetError());
                return;
            }

            mDisplayInfos.clear();
            mDisplayInfos.reserve(numDisplays);

            for (int i = 0; i < numDisplays; ++i)
            {
                SDL_Rect rect;
                SDL_DisplayID displayID = displays[i];
                SDL_GetDisplayBounds(displayID, &rect);

                DisplayInfo info;
                info.width = rect.w;
                info.height = rect.h;

                mDisplayInfos.push_back(info);
            }
        }
    };
}