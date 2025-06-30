#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>

class OpenGLApp
{
public:
    OpenGLApp(int width = 1024, int height = 640, const char* title = "OpenGL App");
    ~OpenGLApp();

    bool initialize();
    void mainLoop();
    void handleEvents();
    void render();
    void cleanup();

private:
    SDL_Window* window;        ///< Pointer to the SDL window
    SDL_Event event;           ///< SDL event structure for handling inputs
    SDL_GLContext mainContext; ///< SDL OpenGL context

    bool quit; ///< Flag indicating if the application should exit
};
