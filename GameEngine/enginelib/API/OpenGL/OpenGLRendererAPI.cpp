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

    void OpenGLRendererAPI::DispatchCompute(uint32_t groupx, uint32_t groupy, uint32_t groupz)
    {
        glDispatchCompute(groupx, groupy, groupz);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }

    void OpenGLRendererAPI::SetDepthTest(bool value)
    {
        if(value)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetDepthWriteMode(bool value)
    {
        glDepthMask(value ? GL_TRUE : GL_FALSE);
    }

    void OpenGLRendererAPI::SetDepthTestFunction(DepthStencilMode criterion)
    {
        glDepthFunc((GLenum)criterion);
    }

    void OpenGLRendererAPI::SetStencilTest(bool value)
    {
        if(value)
            glEnable(GL_STENCIL_TEST);
        else
            glDisable(GL_STENCIL_TEST);
    }

    void OpenGLRendererAPI::SetStencilTestFunction(DepthStencilMode criterion, int8_t refValue, uint8_t mask)
    {
        glStencilFunc((GLenum)criterion, refValue, mask);
    }

    void OpenGLRendererAPI::SetStencilWriteMask(uint32_t mask)
    {
        glStencilMask(mask);
    }

    void OpenGLRendererAPI::SetStencilAction(DepthStencilAction onStencilFail, DepthStencilAction onDepthFail, DepthStencilAction onPass)
    {
        glStencilOp((GLenum)onStencilFail, (GLenum)onDepthFail, (GLenum)onPass);
    }

    void OpenGLRendererAPI::SetCulling(bool value)
    {
        if(value)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }

    void OpenGLRendererAPI::EnableBlend(bool enable)
    {
        if(enable)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    void OpenGLRendererAPI::SetBlendMode(BlendingMode sourceMode, BlendingMode destMode)
    {
        glBlendFunc((GLenum)sourceMode, (GLenum)destMode);
    }

    void OpenGLRendererAPI::SetPointSize(float size)
    {
        glPointSize(size);
    }

    void OpenGLRendererAPI::SetLineWidth(float width)
    {
        glLineWidth(width);
    }

    /*
        std::shared_ptr<Framebuffer> OpenGLRendererAPI::CreateFramebuffer(const FramebufferSettings& settings) {
            return std::make_shared<OpenGLFramebuffer>(settings);
        }


        std::shared_ptr<Texture> OpenGLRendererAPI::CreateTexture(const TextureSettings& settings)
        {
            switch (settings.textureType) {
            case TextureType::Texture2D: return std::make_unique<OpenGLTexture2D>(settings);
            case TextureType::Texture2DArray: return std::make_unique<OpenGLTexture2DArray>(settings);
            case TextureType::TextureCube: return std::make_unique<OpenGLTextureCube>(settings);
            case TextureType::TextureCubeArray: return std::make_unique<OpenGLTextureCubeArray>(settings);
            }
            FTS_ASSERT_MSG(false, "TextureType not supported!");
            return nullptr;
        }


        std::shared_ptr<Shader> OpenGLRendererAPI::CreateShader() {
            return std::make_shared<OpenGLShader>();
        }
        std::shared_ptr<Shader> OpenGLRendererAPI::CreateShader(const std::string& filepath) {
            return std::make_shared<OpenGLShader>(filepath);
        }

        std::shared_ptr<Shader> OpenGLRendererAPI::CreateShader(const std::string& name,
                                                                const std::string& vertexSrc, const std::string& fragmentSrc) {
            return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        std::shared_ptr<Shader> OpenGLRendererAPI::CreateShader(const std::string& name,
                                                                const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geoSrc) {
            return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc, geoSrc);
        }

        std::shared_ptr<Shader> OpenGLRendererAPI::CreateShaderCompile(const std::string& name,
                                                                       const char* vs_source, const char* fs_source, const char* gs_source) {
            return std::make_shared<OpenGLShader>(name, vs_source, fs_source, gs_source);
        }
    */

} // namespace fts
