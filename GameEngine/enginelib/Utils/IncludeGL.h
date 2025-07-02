

#ifdef _WIN32
////#include "GL/glew.h"
// #include "GLFW/glfw3.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#elif __APPLE__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <SDL2/SDL.h>
// #include <SDL_image.h>
// #include <SDL_ttf.h>
