#include <API/OpenGL/OpenGLRendererAPI.h>
#include <Renderer/RendererAPI.h>
#include <ftspch.h>

namespace fts
{

    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch(s_API)
        {
        case RendererAPI::API::None:
            FTS_ASSERT_MSG(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLRendererAPI>();
        }

        FTS_ASSERT_MSG(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace fts
