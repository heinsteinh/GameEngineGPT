#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "Core/Base.h"

#include <glm/glm.hpp>
#include <stdint.h>
#include <string>

namespace fts
{

    enum class MultiSampleLevel : uint32_t
    {
        None = 0,
        X2   = 2,
        X4   = 4,
        X8   = 8
    };

    struct RendererCapabilities
    {
        uint32_t maxTextureUnits;
        uint32_t maxTextureSize;
        std::string version;
        uint32_t shaderVersion;
    };

    enum class FillMode
    {
        Solid = 0,
        WireFrame,
        Point
    };

    enum class CullMode
    {
        None = 0,
        Front,
        Back,
        Front_And_Back
    };

    /* OpenGL specific -> change later */
    enum class DrawMode
    {
        POINTS = 0x0000,
        LINES,
        LINE_LOOP,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,

        LINES_ADJACENCY = 0x000A,
        LINE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY,
        TRIANGLE_STRIP_ADJACENCY,
        PATCHES
    };

    enum class BlendingMode
    {
        ZERO = 0,
        ONE  = 1,

        SRC_COLOR = 0x0300,
        ONE_MINUS_SRC_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA_SATURATE,

        CONSTANT_COLOR = 0x8001,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA
    };

    enum class DepthStencilMode
    {
        NEVER = 0x0200,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS
    };

    enum class DepthStencilAction
    {
        ZERO = 0,
        ONE  = 1,

        KEEP = 0x1E00,
        REPLACE,
        INCREMENT,
        DECREMENT,

        INCR_WRAP = 0x8507,
        DECR_WRAP,

        INVERT = 0x150A
    };

    class RendererAPI
    {
    public:
        enum class API
        {
            None   = 0,
            OpenGL = 1
        };

    public:
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;

        virtual void SetClearColor(const glm::vec4& color)                                                          = 0;
        virtual void Clear()                                                                                        = 0;
        virtual void Clear(bool color, bool depth, bool stencil, uint8_t stencilValue, const glm::vec4& colorValue) = 0;
        virtual void SetCullMode(CullMode mode)                                                                     = 0;
        virtual void SetFrontClockWise(bool enable)                                                                 = 0;

        virtual void SetDepthTest(bool value)                         = 0;
        virtual void SetDepthWriteMode(bool value)                    = 0;
        virtual void SetDepthTestFunction(DepthStencilMode criterion) = 0;

        virtual void SetStencilTest(bool value)                                                                                                                                                                     = 0;
        virtual void SetStencilTestFunction(DepthStencilMode criterion, int8_t refValue, uint8_t mask = 0xff)                                                                                                       = 0;
        virtual void SetStencilWriteMask(uint32_t mask = 0xff)                                                                                                                                                      = 0;
        virtual void SetStencilAction(DepthStencilAction onStencilFail = DepthStencilAction::KEEP, DepthStencilAction onDepthFail = DepthStencilAction::KEEP, DepthStencilAction onPass = DepthStencilAction::KEEP) = 0;

        virtual void SetCulling(bool value)                                       = 0;
        virtual void EnableBlend(bool enable)                                     = 0;
        virtual void SetBlendMode(BlendingMode sourceMode, BlendingMode destMode) = 0;

        // virtual void SetBlendFunc(fts::utils::BlendFunc sourceFunc, fts::utils::BlendFunc destFunc) = 0;

        virtual void SetPointSize(float size)  = 0;
        virtual void SetLineWidth(float width) = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void DispatchCompute(uint32_t groupx, uint32_t groupy, uint32_t groupz)   = 0;

        static API GetAPI()
        {
            return s_API;
        }
        static Scope<RendererAPI> Create();

        inline constexpr const RendererCapabilities& GetCapabilities() const noexcept
        {
            return mCapabilities;
        }

    protected:
        RendererCapabilities mCapabilities;

    private:
        static API s_API;
    };
} // namespace fts

#endif // RENDERERAPI_H
