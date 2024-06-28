#include "VoxelRenderer.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"

namespace Nebula
{

    static constexpr std::uint32_t MAX_INSTANCE_COUNT = 10'000;

    struct VoxelVertex
    {
        glm::vec3 Position;
    };

    struct VoxelInstance
    {
        std::array<std::uint32_t, 3> Offset;

    };

    const std::vector<VoxelVertex> m_Vertices = {
        {{-0.5F, -0.5F, -0.5F}},
        {{0.5F, -0.5F, -0.5F}},
        {{0.5F, 0.5F, -0.5F}},
        {{-0.5F, 0.5F, -0.5F}},
        {{-0.5F, -0.5F, 0.5F}},
        {{0.5F, -0.5F, 0.5F}},
        {{0.5F, 0.5F, 0.5F}},
        {{-0.5F, 0.5F, 0.5F}},
    };

    const std::vector<std::uint32_t> m_Indices = {
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        0, 4, 7, 7, 3, 0, // Left
        1, 5, 6, 6, 2, 1, // Right
        3, 2, 6, 6, 7, 3, // Top
        0, 1, 5, 5, 4, 0, // Bottom
    };

    VoxelRenderer::VoxelRenderer(RefPtr<Window> window, Bounds viewport) :
        BaseRenderer(std::move(window), viewport)

    {
    }

    bool VoxelRenderer::InitComponents()
    {
        std::vector<BufferBinding> vertexInputs(2);
        vertexInputs[0].Binding   = 0;
        vertexInputs[0].Stride    = sizeof(VoxelVertex);
        vertexInputs[0].Instanced = false;
        vertexInputs[0].Elements  = {
            {VertexInput::VertexType::Float, offsetof(VoxelVertex, Position), 3, 0},
        };
        vertexInputs[1].Binding   = 1;
        vertexInputs[1].Stride    = sizeof(VoxelInstance);
        vertexInputs[1].Instanced = true;
        vertexInputs[1].Elements  = {
            {VertexInput::VertexType::UInt, offsetof(VoxelInstance, Offset), 3, 1},
        };

        m_VertexBuffer        = m_RendererAPI->CreateVertexBuffer();
        auto vertexBufferSize = sizeof(VoxelVertex) * m_Vertices.size();
        m_VertexBuffer->Init(m_Context, vertexBufferSize);
        m_InstanceBuffer = m_RendererAPI->CreateVertexBuffer();
        m_InstanceBuffer->Init(m_Context, sizeof(VoxelInstance) * MAX_INSTANCE_COUNT);
        m_IndexBuffer = m_RendererAPI->CreateIndexBuffer();
        m_IndexBuffer->Init(m_Context, m_Indices.size());

        m_UniformBuffer = m_RendererAPI->CreateUniformBuffer();
        m_UniformBuffer->Init(m_Context, sizeof(UniformBufferObject));

        /*InMemoryImage image(File("resources/textures/NoiseMap.jpg"));
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
        */

        std::vector<BindingDescriptor> bindings = {
            BindingDescriptor::Uniform("ViewProjection", 0, sizeof(UniformBufferObject), m_UniformBuffer),
            //BindingDescriptor::TextureSampler("TextureSampler", 1, m_TextureImage, m_TextureSampler),
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

        PipelineShaders shaders;
        shaders.Vertex = "shaders/Voxel_vert.spv";
        shaders.Fragment = "shaders/Voxel_frag.spv";

        std::vector<RefPtr<DescriptorSetLayout>> setLayouts = {m_DescriptorSetLayout};
        m_Pipeline                                          = m_RendererAPI->CreateGraphicsPipeline();
        m_Pipeline->Init(m_Context, shaders, vertexInputs, setLayouts);

        m_VertexBuffer->SetData(m_Context, m_Vertices.data(), vertexBufferSize);
        m_IndexBuffer->SetData(m_Context, m_Indices.data(), m_Indices.size());

        m_UBO.View       = glm::mat4(1.0F);
        m_UBO.Projection = glm::mat4(1.0F);

        return true;
    }

    void VoxelRenderer::Shutdown()
    {
        m_RendererAPI->WaitDeviceIdle();

        m_VertexBuffer->Destroy(m_Context);
        m_IndexBuffer->Destroy(m_Context);
        m_InstanceBuffer->Destroy(m_Context);

        //m_TextureImage->Destroy(m_Context);
        //m_TextureSampler->Destroy(m_Context);
        m_UniformBuffer->Destroy(m_Context);
        m_DescriptorSets->Destroy(m_Context);
        m_DescriptorSetLayout->Destroy(m_Context);
        m_Pipeline->Destroy(m_Context);

        NEBULA_CORE_LOG_INFO("VoxelRenderer shutdown!");
    }

    void VoxelRenderer::BeginFrame()
    {
        InternalBeginFrame();
        NEBULA_PROFILE_SCOPE("VoxelRenderer::BeginFrame");
    }

    void VoxelRenderer::EndFrame() { NEBULA_PROFILE_SCOPE("VoxelRenderer::EndFrame"); }
} // namespace Nebula
