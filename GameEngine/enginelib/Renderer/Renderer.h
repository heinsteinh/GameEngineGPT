#ifndef RENDERER_H
#define RENDERER_H

#include <Camera/Camera.h>
#include <Renderer/RenderCommand.h>

namespace fts
{

    class Texture;
    class Renderer
    {
    public:
        static void Init(float w, float h);
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(Camera& camera);
        static void EndScene();

        static void Submit(const Ref<class Shader>& shader, const Ref<class VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

        static Ref<VertexArray> MakeFullScreenQuad();
        static void DrawFullScreenQuad();
        static void DrawFullScreenQuad(Ref<Texture> texture);

    private:
        static Scope<struct SceneData> s_SceneData;
    };
} // namespace fts

#endif // RENDERER_H
