#include <SDL3/SDL.h>
#include "Core/Logger.hpp"
#include "MainEntry.hpp"
int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD) == false)
        REALSIX_LOG_ERROR("Failed to init SDL3.");

    RealSix::MainEntry(argc, argv);

    SDL_Quit();
    return EXIT_SUCCESS;
}