#include <Renderer/Renderer.h>
#include <ftspch.h>

namespace fts
{

    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
        Ref<Shader> FullQuadShader;
        Ref<VertexArray> FullQuadVao;

        // DrawBuffer mBufferCommand;
        // std::unique_ptr<RenderThread> mRenderThread;
        // std::stack<Rectf> mVpStack;
        // std::stack<Camera> mCamStack;
    };

    Scope<SceneData> Renderer::s_SceneData = CreateScope<SceneData>();

    void Renderer::Init(float w, float h)
    {
        HZ_PROFILE_FUNCTION();

        // Renderer2D::Init();
        // Renderer3D::Init(w, h);
        {

            static const char FULLQUAD_EFFECT_VS[] = {R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec2 aTexCoords;
                out vec2 TexCoords;
                void main()
                {
                    TexCoords = aTexCoords;
                    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                }
             )"};

            static const char FULLQUAD_EFFECT_FS[] = {R"(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoords;
                uniform sampler2D screenTexture;
                void main()
                {
                    vec3 col = texture(screenTexture, TexCoords).rgb;
                    FragColor = vec4(col, 1.0);
                }
             )"};

            // s_SceneData->FullQuadShader = fts::RenderCommand::CreateShaderCompile("FullQuadShader", FULLQUAD_EFFECT_VS, FULLQUAD_EFFECT_FS);
            // s_SceneData->FullQuadVao    = Renderer::MakeFullScreenQuad();
        }
    }

    void Renderer::Shutdown()
    {
        /*
         Renderer2D::Shutdown();
         Renderer3D::Shutdown();

        s_SceneData->FullQuadShader.reset();
        s_SceneData->FullQuadShader = nullptr;

        s_SceneData->FullQuadVao.reset();
        s_SceneData->FullQuadVao = nullptr;
        */
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    /*
    Ref<VertexArray> Renderer::MakeFullScreenQuad()
    {
        struct VertexPT
        {
            glm::vec3 Position;
            glm::vec2 Texcoord;
        };

        std::vector<VertexPT> vertices =
            {
             VertexPT{ {-1.0f, -1.0f, 0.0f},  {0.0f, 0.0f} },
             VertexPT{ { 1.0f, -1.0f, 0.0f},  {1.0f, 0.0f} },
             VertexPT{ { 1.0f,  1.0f, 0.0f},  {1.0f, 1.0f} },
             VertexPT{ {-1.0f,  1.0f, 0.0f},  {0.0f, 1.0f} },
             };

        std::vector<uint32_t> indices =
            {
                0, 1, 2, 2, 3, 0
            };

        VertexBufferLayout layout{
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };

        auto vb = RenderCommand::CreateVertexBuffer(layout, sizeof(VertexPT) * vertices.size(), vertices.data());
        auto ib = RenderCommand::CreateIndexBuffer(indices.size(), indices.data());

        auto vao = RenderCommand::CreateVertexArray();
        vao->AddVertexBuffer(vb);
        vao->AddIndexBuffer(ib);

        return vao;
    }



    void Renderer::BeginScene(Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

        GizmoRenderer::Instance().SetProjection(camera.GetProjectionMatrix());
        GizmoRenderer::Instance().SetView(camera.GetViewMatrix());
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetUniformMatrix("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetUniformMatrix("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }



    void Renderer::DrawFullScreenQuad(Ref<Texture> texture)
    {

        s_SceneData->FullQuadShader->Bind();
        texture->Bind(0);
        s_SceneData->FullQuadShader->SetUniform("screenTexture", 0);

        RenderCommand::DrawIndexed(s_SceneData->FullQuadVao);
    }


    void Renderer::DrawFullScreenQuad()
    {
        RenderCommand::DrawIndexed(s_SceneData->FullQuadVao);
    }
    */
} // namespace fts
