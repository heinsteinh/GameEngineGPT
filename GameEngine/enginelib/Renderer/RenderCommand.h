#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <Renderer/RendererAPI.h>

// #include "Graphic/RenderPrimitives/Framebuffer.h"
// #include "Graphic/RenderPrimitives/Buffer.h"
// #include "Graphic/RenderPrimitives/Texture.h"

namespace fts
{

    struct ContextCreateInfo;
    class Window;
    class RenderCommand
    {

    public:
        static void Init()
        {
            s_RendererAPI->Init();
        }

        static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        static void Clear()
        {
            s_RendererAPI->Clear();
        }

        static void Clear(bool color, bool depth, bool stencil, uint8_t stencilValue = 0, const glm::vec4& colorValue = glm::vec4(1.0f))
        {
            s_RendererAPI->Clear(color, depth, stencil, stencilValue, colorValue);
        }

        static void SetCullMode(CullMode mode)
        {
            s_RendererAPI->SetCullMode(mode);
        }
        static void SetFrontClockWise(bool enable)
        {
            s_RendererAPI->SetFrontClockWise(enable);
        }

        static void SetDepthTest(bool value);
        static void SetDepthWriteMode(bool value);
        static void SetDepthTestFunction(DepthStencilMode criterion);

        static void SetStencilTest(bool value);
        static void SetStencilTestFunction(DepthStencilMode criterion, int8_t refValue, uint8_t mask = 0xff);
        static void SetStencilWriteMask(uint32_t mask = 0xff);
        static void SetStencilAction(DepthStencilAction onStencilFail = DepthStencilAction::KEEP,
                                     DepthStencilAction onDepthFail   = DepthStencilAction::KEEP,
                                     DepthStencilAction onPass        = DepthStencilAction::KEEP);

        static void SetCulling(bool value);
        static void SetPointSize(float size);
        static void SetLineWidth(float width);

        static void EnableBlend(bool enable);

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
} // namespace fts

#endif // RENDERCOMMAND_H
