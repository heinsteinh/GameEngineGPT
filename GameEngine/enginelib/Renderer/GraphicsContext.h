#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include <Core/IWindow.h>
#include <Renderer/RendererAPI.h>

namespace fts
{

    struct ContextCreateInfo
    {
        uint32_t Opengl_Major_Version = 4;
        uint32_t Opengl_Minor_Version = 0;

        MultiSampleLevel MSAA = MultiSampleLevel::X4;
    };

    class Window;
    class GraphicsContext
    {
    public:
        GraphicsContext()          = default;
        virtual ~GraphicsContext() = default;

        virtual void Init()        = 0;
        virtual void BeginFrame()  = 0;
        virtual void EndFrame()    = 0;
        virtual void SwapBuffers() = 0;

        virtual void MakeCurrentContext() const = 0;
        virtual void* GetRenderContext()        = 0;
        virtual void DeleteRenderContext()      = 0;

        static Scope<GraphicsContext> Create(Window* window, const ContextCreateInfo* info);
    };
} // namespace fts

#endif // GRAPHICSCONTEXT_H
