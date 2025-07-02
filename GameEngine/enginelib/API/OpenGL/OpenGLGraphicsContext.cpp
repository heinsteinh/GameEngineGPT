#include <API/OpenGL/OpenGLGraphicsContext.h>
#include <API/Windows/Window_windows.h>
#include <ftspch.h>
// #include <API//OpenGL/OpenGLFramebuffer.h>

namespace fts
{

#ifdef FTS_DEBUG
    void APIENTRY openglCallbackFunction(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {

        std::string severityMessage;
        switch(severity)
        {
        case GL_DEBUG_SEVERITY_LOW:
            severityMessage = "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityMessage = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            severityMessage = "HIGH";
            break;
        }

        switch(type)
        {
        case GL_DEBUG_TYPE_ERROR:
            FTS_CORE_CRITICAL("ERROR {0}", message);
            FTS_CORE_CRITICAL("ID: {0}", id);
            FTS_CORE_CRITICAL("Severity: {0}", severityMessage);
            // FTS_ASSERT(false);
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            FTS_CORE_WARN("DEPRECATED_BEHAVIOR: {0}", message);
            FTS_CORE_WARN("ID: {0}", id);
            FTS_CORE_WARN("Severity: ", severityMessage);
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            FTS_CORE_WARN("UNDEFINED_BEHAVIOR: {0}", message);
            FTS_CORE_WARN("ID: {0}", id);
            FTS_CORE_WARN("Severity: {0}", severityMessage);
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            FTS_CORE_WARN("PORTABILITY: {0}", message);
            FTS_CORE_WARN("ID: {0}", id);
            FTS_CORE_WARN("Severity: {0}", severityMessage);
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            FTS_CORE_WARN("PERFORMANCE: ", message);
            FTS_CORE_WARN("ID: {0}", id);
            FTS_CORE_WARN("Severity: {0}", severityMessage);
            break;
        case GL_DEBUG_TYPE_OTHER:
            FTS_CORE_WARN("WARN {0}", message);
            FTS_CORE_WARN("ID: {0}", id);
            break;
        }
    }
#endif

    OpenGLGraphicsContext::OpenGLGraphicsContext(const Window* wnd, const ContextCreateInfo* info)
        : m_WindowHandle(static_cast<SDL_Window*>(wnd->GetNativeWindow()))
        , m_SDLWindow(wnd)
        , mCtxInfo(info)
    {
        HZ_PROFILE_FUNCTION();
        FTS_ASSERT_MSG(m_WindowHandle, "Window handle is null!");

        m_SDLGLContext = SDL_GL_CreateContext(m_WindowHandle);
    }

    OpenGLGraphicsContext::~OpenGLGraphicsContext()
    {

        SDL_GL_DeleteContext(m_SDLGLContext);
    }

    void OpenGLGraphicsContext::Init()
    {

        HZ_PROFILE_FUNCTION();
        SetOpenGLAttributes();

        SDL_GL_MakeCurrent(m_WindowHandle, m_SDLGLContext);

        /*  if (glewInit() != GLEW_OK)
          {
              throw std::exception("Failed to initialize GLFW.");
          }*/

        // Initialize GLEW
        glewExperimental  = GL_TRUE;
        GLenum glewStatus = glewInit();
        if(glewStatus != GLEW_OK)
        {
            std::cerr << "GLEW Initialization failed: " << glewGetErrorString(glewStatus) << std::endl;

            SDL_Log("Failed to create OpenGL context");
            throw std::runtime_error("Failed to initialize GL extension.");

            exit(-1);
        }

        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        // if (!gladLoadGL())
        // {
        //     SDL_Log("Failed to create OpenGL context");
        //     throw std::exception("Failed to initialize GL extension.");
        //     exit(-1);
        // }

        GLint majorVersion;
        GLint minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

        FTS_CORE_INFO("OpenGL Info:");
        FTS_CORE_INFO("  Version Set: {0}.{0}", majorVersion, minorVersion);
        // FTS_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
        // FTS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        // FTS_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
        // FTS_CORE_INFO("  Version:  {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
        // FTS_CORE_INFO("  Using GLEW: {0}", glewGetString(GLEW_VERSION));

#ifdef FTS_DEBUG
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        FTS_ASSERT_MSG(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Engine requires at least OpenGL version 4.5!");
#endif

#ifdef FTS_DEBUG
        if(glDebugMessageCallback)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glEnable(GL_DEBUG_OUTPUT);

            glDebugMessageCallback(openglCallbackFunction, nullptr);
            GLuint unusedIds = 0;
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
        }
#endif
    }

    void OpenGLGraphicsContext::MakeCurrentContext() const
    {
        SDL_GL_MakeCurrent(m_WindowHandle, m_SDLGLContext);
    }

    void OpenGLGraphicsContext::DeleteRenderContext()
    {
        SDL_GL_DeleteContext(m_SDLGLContext);
    }

    void OpenGLGraphicsContext::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_WindowHandle);
    }

    void OpenGLGraphicsContext::BeginFrame()
    {
        HZ_PROFILE_FUNCTION();

        // MakeCurrentContext();
        {
            // HZ_PROFILE_SCOPE("glBindFramebuffer");
            // GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        {
            HZ_PROFILE_SCOPE("glViewport");
            int width;
            int height;
            SDL_GetWindowSize(m_WindowHandle, &width, &height);
            GLCall(glViewport(0, 0, width, height));
        }
    }

    void OpenGLGraphicsContext::EndFrame()
    {
    }

    void OpenGLGraphicsContext::SetOpenGLAttributes()
    {
        // Set our OpenGL version.
        // 3.3 is part of the modern versions of OpenGL, but most video cards should be able to run it
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, (*mCtxInfo).Opengl_Major_Version);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, (*mCtxInfo).Opengl_Minor_Version);

        // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef FTS_DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_PROFILE_CORE);
#endif // SGL_DEBUG

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Force hardware acceleration

        // Turn on double buffering with a 24bit Z buffer.
        // You may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Enable Double Buffering
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  // 24 bit depth buffer

        // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        // MultiSampling
        if((*mCtxInfo).MSAA != MultiSampleLevel::None)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, static_cast<int>((*mCtxInfo).MSAA));
        }
        else
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        }

        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_GL_SetSwapInterval(0);
    }

    OpenGLFramebufferGC::OpenGLFramebufferGC(OpenGLFramebuffer* framebuffer)
        : m_Framebuffer(framebuffer)
    {
    }

    /*
    void OpenGLFramebufferGC::BeginFrame() {

        Framebuffer::SetCurrentBoundFramebuffer(m_Framebuffer);

        HZ_PROFILE_FUNCTION();
        {
            HZ_PROFILE_SCOPE("glBindFramebuffer");
            glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer->GetRendererId());
        }
        {
            HZ_PROFILE_SCOPE("glViewport");
            glViewport(0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight());
        }
    }


    void OpenGLFramebufferGC::EndFrame() {
        Framebuffer::SetCurrentBoundFramebuffer(nullptr);
        m_Framebuffer->Unbind();
    }


    void OpenGLFramebufferGC::SwapBuffers() {
        HZ_PROFILE_FUNCTION();
        {
            HZ_PROFILE_SCOPE("glBlitFramebuffer");
            static GLenum buffers[4] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
            };

            if (m_Framebuffer->GetResolveRendererId()) {

                GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer->GetRendererId()));
                GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer->GetResolveRendererId()));

                for (uint32_t i = 0; i < m_Framebuffer->GetNumColorAttachments(); ++i) {
                    GLCall(glReadBuffer(buffers[i]));
                    GLCall(glDrawBuffer(buffers[i]));

                    GLCall(glBlitFramebuffer(
                        0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight(),
                        0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight(),
                        GL_COLOR_BUFFER_BIT,
                        GL_NEAREST
                        ));
                }

                if (m_Framebuffer->HasDepthAttachment()) {
                    GLCall(glBlitFramebuffer(
                        0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight(),
                        0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight(),
                        GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
                        GL_NEAREST
                        ));

                }
            }
        }
        {
            HZ_PROFILE_SCOPE("glBindFramebuffer(0)");

            Framebuffer::SetCurrentBoundFramebuffer(nullptr);
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
    }
*/
} // namespace fts
