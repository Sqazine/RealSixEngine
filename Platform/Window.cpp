#include "Window.h"
#include "SDL3/SDL3Window.h"
namespace RealSix
{
    Window *Window::Create()
    {
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
        return new SDL3Window();
#else
#error "Not Support Platform,only windows is available now!"
#endif
    }
}