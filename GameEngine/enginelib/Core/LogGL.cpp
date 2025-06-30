

#include "Core/LogGL.h"

#include "Utils/IncludeGL.h"

/////////////////////////////////////////////////////////////////////////////////////////
void GLClearError()
{
    while(glGetError() != GL_NO_ERROR)
        ;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool GLLogCall(const char* functionName, const char* fileName, int line)
{
#if defined(_DEBUG) || defined(FTS_DEBUG)
    // Clear any previous errors
    GLClearError();
    while(GLenum err = glGetError())
    {

        std::string error;
        switch(err)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        std::stringstream ss;
        ss << "[ OpenGL Error] (" << error << "): " << functionName << " " << fileName << ":" << line;
        fts::Log::GetClientLogger()->error(ss.str());

        return false;
    }
#endif

    return true;
}
