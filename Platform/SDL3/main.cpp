#include <SDL3/SDL.h>
#include "Logger/Logger.h"
#include "Editor/EditorApp.h"
namespace RealSix
{
    extern void MainEntry(int argc, char *argv[]);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD) == false)
        REALSIX_LOG_ERROR(TEXT("Failed to init SDL3."));

    RealSix::MainEntry(argc, argv);

    SDL_Quit();
    return EXIT_SUCCESS;
}