#ifndef OPENGLGRAPHICSCONTEXT_H
#define OPENGLGRAPHICSCONTEXT_H

#pragma once

#include <Renderer/GraphicsContext.h>
#include <Utils/IncludeGL.h>

namespace fts
{
    class Window;
    class OpenGLGraphicsContext : public GraphicsContext
    {
        using super = GraphicsContext;

    public:
        OpenGLGraphicsContext(const Window* window, const ContextCreateInfo* info);
        ~OpenGLGraphicsContext();

        virtual void Init() override;
        virtual void BeginFrame() override;
        virtual void EndFrame() override;
        virtual void SwapBuffers() override;

        virtual void MakeCurrentContext() const override;
        virtual void* GetRenderContext() override
        {
            return m_SDLGLContext;
        };
        virtual void DeleteRenderContext() override;

    private:
        const Window* m_SDLWindow;
        SDL_Window* m_WindowHandle;
        void* m_SDLGLContext;
        const ContextCreateInfo* mCtxInfo;
        bool anotherWin = false;

        void SetOpenGLAttributes();

        friend class Framebuffer;
    };

    class OpenGLFramebuffer;
    class OpenGLFramebufferGC : public fts::GraphicsContext
    {

    public:
        OpenGLFramebufferGC(OpenGLFramebuffer* framebuffer); // raw pointer is fine here.  We know the OpenGLFramebufferGC lifetime is nested inside the framebuffer's lifetime

        virtual void Init() override {};

        virtual void MakeCurrentContext() const override
        {
        }
        virtual void* GetRenderContext() override
        {
            return nullptr;
        }
        virtual void DeleteRenderContext() override
        {
        }

        virtual void BeginFrame() override;
        virtual void EndFrame() override;
        virtual void SwapBuffers() override;

    private:
        OpenGLFramebuffer* m_Framebuffer;
    };
} // namespace fts

#endif // OPENGLGRAPHICSCONTEXT_H
