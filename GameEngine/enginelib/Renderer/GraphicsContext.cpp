#include <API/OpenGL/OpenGLGraphicsContext.h>
#include <Core/IWindow.h>
#include <Renderer/GraphicsContext.h>
#include <Renderer/Renderer.h>
#include <ftspch.h>

namespace fts
{

    Scope<GraphicsContext> GraphicsContext::Create(Window* window, const ContextCreateInfo* info)
    {
        switch(Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            FTS_ASSERT_MSG(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLGraphicsContext>(window, info);
        }

        FTS_ASSERT_MSG(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace fts
