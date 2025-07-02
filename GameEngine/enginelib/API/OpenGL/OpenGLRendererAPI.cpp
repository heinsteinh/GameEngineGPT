#include <API/OpenGL/OpenGLRendererAPI.h>
#include <Renderer/GraphicsContext.h>
#include <Utils/IncludeGL.h>
#include <ftspch.h>

#define LOG_GL_NOTIFICATIONS

namespace fts
{

    namespace Utils
    {
        /* GLenum DrawModetoGL(DrawMode mode)
         {
             switch (mode)
             {
             case DrawMode::POINT:			return GL_POINTS;
             case DrawMode::LINES:			return GL_LINES;
             case DrawMode::LINE_LOOP:		return GL_LINE_LOOP;
             case DrawMode::LINE_STRIP:		return GL_LINE_STRIP;
             case DrawMode::TRIANGLES:		return GL_TRIANGLES;
             case DrawMode::TRIANGLES_FAN:	return GL_TRIANGLE_FAN;
             case DrawMode::TRIANGLE_STRIP:	return GL_TRIANGLE_STRIP;
             }
         }*/

        GLenum CullModeToGL(CullMode mode)
        {
            switch(mode)
            {
            case CullMode::Back:
                return GL_BACK;
            case CullMode::Front:
                return GL_FRONT;
            case CullMode::Front_And_Back:
                return GL_FRONT_AND_BACK;
                FTS_ASSERT_MSG(false, "Wrong Cullmode value.");
            }
        }

        GLenum FillModeToGL(FillMode mode)
        {
            switch(mode)
            {
            case FillMode::Solid:
                return GL_FILL;
            case FillMode::WireFrame:
                return GL_LINE;
                FTS_ASSERT_MSG(false, "Wrong Fillmode value.");
            }
        }
    } // namespace Utils

    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
    {
        switch(severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            FTS_CORE_CRITICAL(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            FTS_CORE_ERROR(message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            FTS_CORE_WARN(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            FTS_CORE_TRACE(message);
            return;
        }

        FTS_ASSERT_MSG(false, "Unknown severity level!");
    }

    void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                        const GLchar* message, const void* userParam)
    {
        std::string sourceTxt;
        switch(source)
        {
        case GL_DEBUG_SOURCE_API:
            sourceTxt = "DEBUG";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceTxt = "WINDOW";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceTxt = "SHADER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceTxt = "THIRD PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceTxt = "APP";
            break;
        case GL_DEBUG_SOURCE_OTHER:
        default:
            sourceTxt = "OTHER";
            break;
        }
        switch(severity)
        {
        case GL_DEBUG_SEVERITY_LOW:
            FTS_CORE_INFO("[{}] {}", sourceTxt, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            FTS_CORE_WARN("[{}] {}", sourceTxt, message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            FTS_CORE_ERROR("[{}] {}", sourceTxt, message);
            break;

#ifdef LOG_GL_NOTIFICATIONS
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            FTS_CORE_INFO("[{}] {}", sourceTxt, message);
            break;
#endif
        default:
            break;
        }
    }

    void OpenGLRendererAPI::Init()
    {
        HZ_PROFILE_FUNCTION();

#ifdef FTS_DEBUG

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GlDebugMessage, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        GLfloat pointSize;
        glGetFloatv(GL_POINT_SIZE, &pointSize);
        FTS_CORE_INFO("Default point size: {0}", pointSize);
        glPointSize(4);
        glGetFloatv(GL_POINT_SIZE, &pointSize);
        FTS_CORE_INFO("New point size: {0}", pointSize);

        GLfloat lineWidth;
        glGetFloatv(GL_LINE_WIDTH, &lineWidth);
        FTS_CORE_INFO("Default line width: {0}", lineWidth);
        glLineWidth(2);
        glGetFloatv(GL_LINE_WIDTH, &lineWidth);
        FTS_CORE_INFO("New line width: {0}", lineWidth);

        GLfloat maxAniso;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
        FTS_CORE_INFO("Max Anisotropy value: {0}", maxAniso);

        glEnable(GL_DEPTH_TEST);
        /* glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glEnable(GL_MULTISAMPLE);*/

        FTS_CORE_TRACE("RendererAPI (OpenGL) initialized successfully!");
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void OpenGLRendererAPI::Clear(bool color, bool depth, bool stencil, uint8_t stencilValue, const glm::vec4& colorValue)
    {
        int clearFlags = 0;
        if(color)
        {
            glClearColor(colorValue.r, colorValue.g, colorValue.b, colorValue.a);
            clearFlags |= GL_COLOR_BUFFER_BIT;
        }
        if(depth)
        {
            clearFlags |= GL_DEPTH_BUFFER_BIT;
        }
        if(stencil)
        {
            glClearStencil(stencilValue);
            clearFlags |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(clearFlags);
    }

    void OpenGLRendererAPI::SetCullMode(CullMode mode)
    {
        if(mode == CullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            GLenum cullMode = Utils::CullModeToGL(mode);
            glCullFace(cullMode);
        }
    }

    void OpenGLRendererAPI::SetFrontClockWise(bool enable)
    {

        if(enable)
            glFrontFace(GL_CCW);
        else
            glFrontFace(GL_CW);
    }

} // namespace fts
