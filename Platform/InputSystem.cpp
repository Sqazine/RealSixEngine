#include "InputSystem.h"
#include "SDL3/SDL3InputSystem.h"
namespace RealSix
{
    InputSystem *InputSystem::Create()
    {
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
        return new SDL3InputSystem();
#else
#error "Not Support Platform, only windows is available now!"
#endif
    }
}