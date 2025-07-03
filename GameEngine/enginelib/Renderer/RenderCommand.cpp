#include <Renderer/RenderCommand.h>
#include <ftspch.h>

namespace fts
{

    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

    void RenderCommand::SetDepthTest(bool value)
    {
        s_RendererAPI->SetDepthTest(value);
    }

    void RenderCommand::SetDepthWriteMode(bool value)
    {
        s_RendererAPI->SetDepthWriteMode(value);
    }

    void RenderCommand::SetDepthTestFunction(DepthStencilMode criterion)
    {
        s_RendererAPI->SetDepthTestFunction(criterion);
    }

    void RenderCommand::SetStencilTest(bool value)
    {
        s_RendererAPI->SetStencilTest(value);
    }

    void RenderCommand::SetStencilTestFunction(DepthStencilMode criterion, int8_t refValue, uint8_t mask)
    {
        s_RendererAPI->SetStencilTestFunction(criterion, refValue, mask);
    }

    void RenderCommand::SetStencilWriteMask(uint32_t mask)
    {
        s_RendererAPI->SetStencilWriteMask(mask);
    }

    void RenderCommand::SetStencilAction(DepthStencilAction onStencilFail, DepthStencilAction onDepthFail, DepthStencilAction onPass)
    {
        s_RendererAPI->SetStencilAction(onStencilFail, onDepthFail, onPass);
    }

    void RenderCommand::SetCulling(bool value)
    {
        s_RendererAPI->SetCulling(value);
    }

    void RenderCommand::EnableBlend(bool enable)
    {
        s_RendererAPI->EnableBlend(enable);
    }

    void RenderCommand::SetPointSize(float size)
    {
        s_RendererAPI->SetPointSize(size);
    }

    void RenderCommand::SetLineWidth(float width)
    {
        s_RendererAPI->SetLineWidth(width);
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

} // namespace fts
