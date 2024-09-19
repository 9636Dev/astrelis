#include "Renderer2D.hpp"

#include "Astrelis/Core/Base.hpp"

#include "Astrelis/Core/GlobalConfig.hpp"
#include "Astrelis/Renderer/BindingDescriptor.hpp"
#include "Astrelis/Renderer/ShaderFormat.hpp"

#include "GraphicsPipeline.hpp"

namespace Astrelis {
    static constexpr std::uint32_t MAX_INSTANCE_COUNT = 1'000;

    Renderer2D::Renderer2D(RefPtr<Window> window, Rect2Di viewport)
        : BaseRenderer(std::move(window), viewport) {
        ASTRELIS_PROFILE_FUNCTION();
        m_Instances.reserve(MAX_INSTANCE_COUNT);
    }

    bool Renderer2D::InitComponents() {
        ASTRELIS_PROFILE_FUNCTION();
        std::vector<BufferBinding> vertexInputs(2);
        vertexInputs[0].Binding   = 0;
        vertexInputs[0].Stride    = sizeof(Vertex2D);
        vertexInputs[0].Instanced = false;
        vertexInputs[0].Elements  = {
            {VertexInput::VertexType::Float, offsetof(Vertex2D, Position), 3, 0},
            {VertexInput::VertexType::Float, offsetof(Vertex2D, TexCoord), 2, 1},
        };
        vertexInputs[1].Binding   = 1;
        vertexInputs[1].Stride    = sizeof(InstanceData);
        vertexInputs[1].Instanced = true;
        vertexInputs[1].Elements  = {
            {VertexInput::VertexType::Float, 0, 4, 2},
            {VertexInput::VertexType::Float, 4 * sizeof(float), 4, 3},
            {VertexInput::VertexType::Float, 8 * sizeof(float), 4, 4},
            {VertexInput::VertexType::Float, 12 * sizeof(float), 4, 5},
            {VertexInput::VertexType::Float, offsetof(InstanceData, Color), 3, 6},
        };


        File shader("resources/shaders/Basic.astshader");
        ASTRELIS_VERIFY(shader.Exists(), "Shader file does not exist!");
        auto res = shader.ReadBinaryStructure<ShaderFormat>();
        if (res.IsErr()) {
            ASTRELIS_CORE_LOG_ERROR("Failed to read shader file: {0}", res.UnwrapErr());
            return false;
        }
        ShaderFormat shaderFormat = std::move(res.Unwrap());

        if (GlobalConfig::IsDebugMode()) {
            ASTRELIS_CORE_LOG_DEBUG("Loaded version {0} shader from: {1}",
                shaderFormat.Header.FileVersion, shader.GetPath().string());
            ASTRELIS_CORE_LOG_DEBUG("Shader name: {0}", shaderFormat.Header.Name);
            ASTRELIS_CORE_LOG_DEBUG("Shader flags: {0}",
                static_cast<std::underlying_type_t<ShaderHeader::HeaderFlags>>(
                    shaderFormat.Header.Flags));
        }

        std::vector<char> vertexData;
        std::vector<char> fragmentData;

        for (const auto& [stage, shader] : shaderFormat.Source.SourceSPIRV.Shaders) {
            if (stage == ShaderStage::Vertex) {
                vertexData = std::vector<char>(reinterpret_cast<const char*>(shader.Data.data()),
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                    reinterpret_cast<const char*>(shader.Data.data() + shader.Data.size()));
            }
            else if (stage == ShaderStage::Fragment) {
                fragmentData = std::vector<char>(reinterpret_cast<const char*>(shader.Data.data()),
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                    reinterpret_cast<const char*>(shader.Data.data() + shader.Data.size()));
            }
        }

        ASTRELIS_VERIFY(!vertexData.empty(), "Vertex shader data is empty!");
        ASTRELIS_VERIFY(!fragmentData.empty(), "Fragment shader data is empty!");

        CompiledShader vertexCompiled(CompiledShader::VulkanShader(vertexData, "VS_Main"));
        CompiledShader fragmentCompiled(CompiledShader::VulkanShader(fragmentData, "PS_Main"));

        PipelineShaders shaders(vertexCompiled, fragmentCompiled);

        m_VertexBuffer        = m_RendererAPI->CreateVertexBuffer();
        auto vertexBufferSize = sizeof(Vertex2D) * 1000;
        m_VertexBuffer->Init(m_Context, vertexBufferSize);
        m_InstanceBuffer = m_RendererAPI->CreateVertexBuffer();
        m_InstanceBuffer->Init(m_Context, sizeof(InstanceData) * MAX_INSTANCE_COUNT);
        m_IndexBuffer = m_RendererAPI->CreateIndexBuffer();
        m_IndexBuffer->Init(m_Context, 1000);

        m_UniformBuffer = m_RendererAPI->CreateUniformBuffer();
        m_UniformBuffer->Init(m_Context, sizeof(CameraUniformData));

        const std::vector<DescriptorSetBinding> bindings = {
            DescriptorSetBinding("MVP", DescriptorType::Uniform, 0,
                DescriptorSetBinding::StageFlags::Vertex, sizeof(CameraUniformData),
                {{m_UniformBuffer.Raw()}}),
        };


        m_Bindings = m_RendererAPI->CreateBindingDescriptorSet(BindingDescriptorSet::Mode::One);
        if (!m_Bindings->Init(m_Context, bindings)) {
            return false;
        }

        std::vector<RawRef<BindingDescriptorSet*>> setLayouts = {m_Bindings.Raw()};
        m_Pipeline = m_RendererAPI->CreateGraphicsPipeline();
        m_Pipeline->Init(m_Context, shaders, vertexInputs, setLayouts, PipelineType::Graphics);

        m_UBO.View       = Mat4f(1.0F);
        m_UBO.Projection = Mat4f(1.0F);

        return true;
    }

    void Renderer2D::Shutdown() {
        ASTRELIS_PROFILE_FUNCTION();
        m_RendererAPI->WaitDeviceIdle();

        m_VertexBuffer->Destroy(m_Context);
        m_IndexBuffer->Destroy(m_Context);
        m_InstanceBuffer->Destroy(m_Context);

        m_UniformBuffer->Destroy(m_Context);
        m_Bindings->Destroy(m_Context);
        m_Pipeline->Destroy(m_Context);

        ASTRELIS_CORE_LOG_INFO("Renderer2D shutdown!");
    }

    void Renderer2D::BeginFrame() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Renderer2D::BeginFrame");
        InternalBeginFrame();

        m_VertexBuffer->Bind(m_Context, 0);
        m_InstanceBuffer->Bind(m_Context, 1);
        m_IndexBuffer->Bind(m_Context);
        m_UniformBuffer->SetData(m_Context, &m_UBO, sizeof(CameraUniformData), 0);
        m_Bindings->Bind(m_Context, m_Pipeline);
    }

    void Renderer2D::Submit(const Mesh2D& mesh, InstanceData instance) {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Renderer2D::Submit");

        m_Instances.clear();
        m_Instances.push_back(instance);

        m_VertexBuffer->SetData(m_Context, mesh.Vertices.data(),
            static_cast<std::uint32_t>(mesh.Vertices.size() * sizeof(Vertex2D)));
        m_IndexBuffer->SetData(m_Context, mesh.Indices.data(),
            static_cast<std::uint32_t>(mesh.Indices.size() * sizeof(Mesh2D::IndicesType)));
        m_InstanceBuffer->SetData(m_Context, m_Instances.data(),
            static_cast<std::uint32_t>(m_Instances.size() * sizeof(InstanceData)));

        m_RendererAPI->DrawInstancedIndexed(mesh.Indices.size(), 1, 0, 0, 0);
    }

    void Renderer2D::EndFrame() {
        ASTRELIS_PROFILE_SCOPE("Astrelis::Renderer2D::EndFrame");
    }
} // namespace Astrelis
