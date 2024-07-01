#include "Renderer2D.hpp"

#include <utility>

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/IO/Image.hpp"
#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"
#include "NebulaEngine/Scene/TransformComponent.hpp"
#include "Vertex.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Nebula
{
    static constexpr std::uint32_t MAX_INSTANCE_COUNT = 1'000;

    const std::vector<Vertex2D> m_Vertices = {
        {{-0.5F, -0.5F, 0.0F}, {1.0F, 0.0F, 0.0F}, {0.0F, 0.0F}},
        {{0.5F, -0.5F, 0.0F},  {0.0F, 1.0F, 0.0F}, {1.0F, 0.0F}},
        {{0.5F, 0.5F, 0.0F},   {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}},
        {{-0.5F, 0.5F, 0.0F},  {1.0F, 1.0F, 1.0F}, {0.0F, 1.0F}}
    };
    const std::vector<std::uint32_t> m_Indices = {0, 1, 2, 2, 3, 0};

    Renderer2D::Renderer2D(RefPtr<Window> window, Bounds viewport) : BaseRenderer(std::move(window), viewport)
    {
        m_Instances.reserve(MAX_INSTANCE_COUNT);
    }

    bool Renderer2D::InitComponents()
    {
        std::vector<BufferBinding> vertexInputs(2);
        vertexInputs[0].Binding   = 0;
        vertexInputs[0].Stride    = sizeof(Vertex2D);
        vertexInputs[0].Instanced = false;
        vertexInputs[0].Elements  = {
            {VertexInput::VertexType::Float, offsetof(Vertex2D, Position), 3, 0},
            {VertexInput::VertexType::Float, offsetof(Vertex2D, Color),    3, 1},
            {VertexInput::VertexType::Float, offsetof(Vertex2D, TexCoord), 2, 2},
        };
        vertexInputs[1].Binding   = 1;
        vertexInputs[1].Stride    = sizeof(InstanceData);
        vertexInputs[1].Instanced = true;
        vertexInputs[1].Elements  = {
            {VertexInput::VertexType::Float, 0,                  4, 3},
            {VertexInput::VertexType::Float, 4 * sizeof(float),  4, 4},
            {VertexInput::VertexType::Float, 8 * sizeof(float),  4, 5},
            {VertexInput::VertexType::Float, 12 * sizeof(float), 4, 6},
        };

        PipelineShaders shaders;
        shaders.Vertex   = "resources/shaders/Basic_vert.spv";
        shaders.Fragment = "resources/shaders/Basic_frag.spv";

        m_VertexBuffer        = m_RendererAPI->CreateVertexBuffer();
        auto vertexBufferSize = sizeof(Vertex2D) * m_Vertices.size();
        m_VertexBuffer->Init(m_Context, vertexBufferSize);
        m_InstanceBuffer = m_RendererAPI->CreateVertexBuffer();
        m_InstanceBuffer->Init(m_Context, sizeof(InstanceData) * MAX_INSTANCE_COUNT);
        m_IndexBuffer = m_RendererAPI->CreateIndexBuffer();
        m_IndexBuffer->Init(m_Context, m_Indices.size());

        m_UniformBuffer = m_RendererAPI->CreateUniformBuffer();
        m_UniformBuffer->Init(m_Context, sizeof(CameraUniformData));

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
            BindingDescriptor::Uniform("MVP", 0, sizeof(CameraUniformData), m_UniformBuffer),
            BindingDescriptor::TextureSampler("TextureSampler", 1, m_TextureImage, m_TextureSampler),
        };

        m_DescriptorSetLayout = m_RendererAPI->CreateDescriptorSetLayout();
        if (!m_DescriptorSetLayout->Init(m_Context, bindings))
        {
            return false;
        }

        m_DescriptorSets = m_RendererAPI->CreateDescriptorSets();
        if (!m_DescriptorSets->Init(m_Context, m_DescriptorSetLayout, bindings))
        {
            return false;
        }

        std::vector<RefPtr<DescriptorSetLayout>> setLayouts = {m_DescriptorSetLayout};
        m_Pipeline                                          = m_RendererAPI->CreateGraphicsPipeline();
        m_Pipeline->Init(m_Context, shaders, vertexInputs, setLayouts);

        m_VertexBuffer->SetData(m_Context, m_Vertices.data(), vertexBufferSize);
        m_IndexBuffer->SetData(m_Context, m_Indices.data(), m_Indices.size());

        m_UBO.Model      = glm::mat4(1.0F);
        m_UBO.View       = glm::mat4(1.0F);
        m_UBO.Projection = glm::mat4(1.0F);

        std::vector<InstanceData> instanceData = {
            {glm::translate(glm::mat4(1.0F), glm::vec3(-0.5F, -0.5F, 0.0F))},
            {glm::translate(glm::mat4(1.0F), glm::vec3(0.5F, 0.5F, 0.0F))},
        };
        m_InstanceBuffer->SetData(m_Context, instanceData.data(), sizeof(InstanceData) * 2);

        return true;
    }

    void Renderer2D::Shutdown()
    {
        m_RendererAPI->WaitDeviceIdle();

        m_VertexBuffer->Destroy(m_Context);
        m_IndexBuffer->Destroy(m_Context);
        m_InstanceBuffer->Destroy(m_Context);

        m_TextureImage->Destroy(m_Context);
        m_TextureSampler->Destroy(m_Context);
        m_UniformBuffer->Destroy(m_Context);
        m_DescriptorSets->Destroy(m_Context);
        m_DescriptorSetLayout->Destroy(m_Context);
        m_Pipeline->Destroy(m_Context);

        NEBULA_CORE_LOG_INFO("Renderer2D shutdown!");
    }

    void Renderer2D::BeginFrame()
    {
        InternalBeginFrame();
        NEBULA_PROFILE_SCOPE("Renderer2D::BeginFrame");
    }

    void Renderer2D::RenderScene(Scene2D& scene, Camera& camera)
    {
        NEBULA_PROFILE_SCOPE("Renderer2D::RenderScene");
        m_Instances.clear();

        m_UBO.Model        = glm::rotate(m_UBO.Model, glm::radians(0.1F), glm::vec3(0.0F, 0.0F, 1.0F));
        m_UBO.View         = camera.GetViewMatrix();
        Bounds surfaceSize = m_RendererAPI->GetSurfaceSize();
        float aspectRatio  = static_cast<float>(surfaceSize.Width) / static_cast<float>(surfaceSize.Height);
        m_UBO.Projection   = glm::ortho(-aspectRatio, aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F);
        m_RendererAPI->CorrectProjection(m_UBO.Projection);

        m_VertexBuffer->Bind(m_Context, 0);
        m_InstanceBuffer->Bind(m_Context, 1);
        m_IndexBuffer->Bind(m_Context);
        m_UniformBuffer->SetData(m_Context, &m_UBO, sizeof(CameraUniformData), 0);
        m_DescriptorSets->Bind(m_Context, m_Pipeline);

        for (auto entity : scene.GetComponents<TransformComponent>())
        {
            m_Instances.push_back({scene.GetComponent<TransformComponent>(entity).Transform});
            if (m_Instances.size() >= MAX_INSTANCE_COUNT)
            {
                DrawInstances();
                m_Instances.clear();
            }
        }

        DrawInstances();
    }

    void Renderer2D::DrawInstances()
    {
        if (m_Instances.empty())
        {
            return;
        }
        m_InstanceBuffer->SetData(m_Context, m_Instances.data(), sizeof(InstanceData) * m_Instances.size());
        m_RendererAPI->DrawInstancedIndexed(static_cast<std::uint32_t>(m_Indices.size()), m_Instances.size(), 0, 0, 0);
    }

    void Renderer2D::EndFrame() { NEBULA_PROFILE_SCOPE("Renderer2D::EndFrame"); }


} // namespace Nebula
