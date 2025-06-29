#include "OpenGLApp.h"

OpenGLApp::OpenGLApp(int width, int height, const char* title)
    : window(nullptr), quit(false), mainContext(nullptr)
{
    // Set SDL_GL attributes before creating the window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create SDL window
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
    }
}

bool OpenGLApp::initialize()
{
    if (!window) return false;

    mainContext = SDL_GL_CreateContext(window);
    if (!mainContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "GLEW Initialization failed: " << glewGetErrorString(glewStatus) << std::endl;
        return false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void OpenGLApp::mainLoop()
{
    while (!quit) {
        handleEvents();
        render();
        SDL_GL_SwapWindow(window);
    }
}

void OpenGLApp::handleEvents()
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
        }
    }
}

void OpenGLApp::render()
{
    // Clear screen
    glClearColor(0.1f, 0.15f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw stuff here
}

void OpenGLApp::cleanup()
{
    if (mainContext) {
        SDL_GL_DeleteContext(mainContext);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

OpenGLApp::~OpenGLApp()
{
    cleanup();
}
