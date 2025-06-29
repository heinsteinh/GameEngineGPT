#include "OpenGLApp.h"

#include <spdlog/spdlog.h>

int main()
{
    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    OpenGLApp app;
    if (!app.initialize()) {
        return -1;
    }

    app.mainLoop();
    return 0;
}
