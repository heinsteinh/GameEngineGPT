#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include <Renderer/RendererAPI.h>

// #include "Graphic/RenderPrimitives/Framebuffer.h"
// #include "Graphic/RenderPrimitives/Buffer.h"
// #include "Graphic/RenderPrimitives/Texture.h"

namespace fts
{

    struct ContextCreateInfo;
    class VertexArray;
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;

        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void Clear(bool color, bool depth, bool stencil, uint8_t stencilValue, const glm::vec4& colorValue) override;

        virtual void SetCullMode(CullMode mode) override;
        virtual void SetFrontClockWise(bool enable) override;

        void SetDepthTest(bool value) override;
        void SetDepthWriteMode(bool value) override;
        void SetDepthTestFunction(DepthStencilMode criterion) override;

        void SetStencilTest(bool value) override;
        void SetStencilTestFunction(DepthStencilMode criterion, int8_t refValue, uint8_t mask) override;
        void SetStencilWriteMask(uint32_t mask) override;
        void SetStencilAction(DepthStencilAction onStencilFail = DepthStencilAction::KEEP, DepthStencilAction onDepthFail = DepthStencilAction::KEEP, DepthStencilAction onPass = DepthStencilAction::KEEP) override;

        void SetCulling(bool value) override;
        void EnableBlend(bool enable) override;
        void SetBlendMode(BlendingMode sourceMode, BlendingMode destMode) override;

        // void SetBlendFunc(fts::utils::BlendFunc sourceFunc, fts::utils::BlendFunc destFunc) override;

        // void SetBlendMode(fts::utils::BlendMode mode, bool alphaToCoverage);
        // void SetDepthTestFunc(fts::utils::CompareFunc depthFunc);
        // void SetDepthTestWrite(bool enable);

        void SetPointSize(float size) override;
        void SetLineWidth(float width) override;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void DispatchCompute(uint32_t groupx, uint32_t groupy, uint32_t groupz) override;
    };

} // namespace fts

#endif // OPENGLRENDERERAPI_H
