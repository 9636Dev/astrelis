#include "Renderer2D.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/IO/Image.hpp"
#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"
#include "Vertex.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Nebula
{
    const std::vector<Vertex2D> m_Vertices = {
        {{-0.5F, -0.5F, 0.0F}, {1.0F, 0.0F, 0.0F}, {0.0F, 0.0F}},
        {{0.5F, -0.5F, 0.0F},  {0.0F, 1.0F, 0.0F}, {1.0F, 0.0F}},
        {{0.5F, 0.5F, 0.0F},   {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}},
        {{-0.5F, 0.5F, 0.0F},  {1.0F, 1.0F, 1.0F}, {0.0F, 1.0F}}
    };
    const std::vector<std::uint32_t> m_Indices = {0, 1, 2, 2, 3, 0};

    Renderer2D::Renderer2D(RefPtr<Window> window, Bounds viewport) :
        m_Window(std::move(window)),
        m_Context(m_Window->GetGraphicsContext()),
        m_RendererAPI(RendererAPI::Create(m_Context, viewport, RendererAPI::Type::Renderer2D)),
        m_DescriptorCount(0)
    {
    }

    bool Renderer2D::Init()
    {
        m_RendererAPI->Init();
        RendererAPI::CreateDetails details;
        details.VertexBufferSize     = m_Vertices.size() * sizeof(Vertex2D);
        details.IndicesCount         = m_Indices.size();
        details.VertexInput.Stride   = sizeof(Vertex2D);
        details.VertexInput.Elements = {
            {offsetof(Vertex2D, Position), 3, 0},
            {offsetof(Vertex2D, Color),    3, 0},
            {offsetof(Vertex2D, TexCoord), 2, 0},
        };

        m_UniformBuffer = m_RendererAPI->CreateUniformBuffer();
        m_UniformBuffer->Init(m_Context, sizeof(UniformBufferObject));

        InMemoryImage image(File("resources/textures/NoiseMap.jpg"));
        m_TextureImage = m_RendererAPI->CreateTextureImage();
        if (!m_TextureImage->LoadTexture(m_Context, image))
        {
            return false;
        }

        m_TextureSampler = m_RendererAPI->CreateTextureSampler();
        if (!m_TextureSampler->Init(m_Context))
        {
            return false;
        }

        std::vector<BindingDescriptor> bindings = {
            BindingDescriptor::Uniform("MVP", 0, sizeof(UniformBufferObject), m_UniformBuffer),
            BindingDescriptor::TextureSampler("TextureSampler", 1, m_TextureImage, m_TextureSampler),
        };

        m_DescriptorSetLayout = m_RendererAPI->CreateDescriptorSetLayout();
        if (!m_DescriptorSetLayout->Init(m_Context, bindings))
        {
            return false;
        }
        details.DescriptorSetLayouts = {m_DescriptorSetLayout};

        m_DescriptorSets = m_RendererAPI->CreateDescriptorSets();
        if (!m_DescriptorSets->Init(m_Context, m_DescriptorSetLayout, bindings))
        {
            return false;
        }

        m_Storage = m_RendererAPI->CreateComponents(details);

        m_Storage.m_VertexBuffer->SetData(m_Context, m_Vertices.data(), details.VertexBufferSize);
        m_Storage.m_IndexBuffer->SetData(m_Context, m_Indices.data(), details.IndicesCount);

        m_UBO.Model      = glm::mat4(1.0F);
        m_UBO.View       = glm::mat4(1.0F);
        m_UBO.Projection = glm::mat4(1.0F);

        return true;
    }

    void Renderer2D::Shutdown()
    {
        m_RendererAPI->WaitDeviceIdle();
        m_TextureImage->Destroy(m_Context);
        m_TextureSampler->Destroy(m_Context);
        m_UniformBuffer->Destroy(m_Context);
        m_DescriptorSets->Destroy(m_Context);
        m_DescriptorSetLayout->Destroy(m_Context);
        m_RendererAPI->DestroyComponents(m_Storage);
        m_RendererAPI->Shutdown();

        NEBULA_CORE_LOG_INFO("Renderer2D shutdown!");
    }

    void Renderer2D::BeginFrame()
    {
        NEBULA_PROFILE_SCOPE("Renderer2D::BeginFrame");

        m_Storage.m_GraphicsPipeline->Bind(m_Context);
        Bounds scissor = m_RendererAPI->GetSurfaceSize();
        Viewport viewport(0.0F, 0.0F, static_cast<float>(scissor.Width), static_cast<float>(scissor.Height));
        m_RendererAPI->SetViewport(viewport);
        m_RendererAPI->SetScissor(scissor);
    }

    void Renderer2D::RenderScene(Scene& scene, Camera& camera)
    {
        NEBULA_PROFILE_SCOPE("Renderer2D::RenderScene");
        (void)scene;

        m_UBO.Model        = glm::rotate(m_UBO.Model, glm::radians(0.1F), glm::vec3(0.0F, 0.0F, 1.0F));
        m_UBO.View         = camera.GetViewMatrix();
        Bounds surfaceSize = m_RendererAPI->GetSurfaceSize();
        float aspectRatio  = static_cast<float>(surfaceSize.Width) / static_cast<float>(surfaceSize.Height);
        m_UBO.Projection   = glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F);
        m_RendererAPI->CorrectProjection(m_UBO.Projection);

        m_Storage.m_VertexBuffer->Bind(m_Context);
        m_Storage.m_IndexBuffer->Bind(m_Context);
        m_UniformBuffer->SetData(m_Context, &m_UBO, sizeof(UniformBufferObject), 0);
        m_DescriptorSets->Bind(m_Context, m_Storage.m_GraphicsPipeline);
        m_RendererAPI->DrawInstancedIndexed(static_cast<std::uint32_t>(m_Indices.size()), 1, 0, 0, 0);
    }

    void Renderer2D::EndFrame() { NEBULA_PROFILE_SCOPE("Renderer2D::EndFrame"); }

    void Renderer2D::ResizeViewport() { m_RendererAPI->ResizeViewport(); }

} // namespace Nebula
