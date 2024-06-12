#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Base.hpp"
#include "NebulaEngine/Core/Time.hpp"
#include "imgui.h"
#include <random>

SandboxLayer::SandboxLayer()
{
    m_Mesh.Vertices = {
        { { -0.5F, -0.5F, 0.0F }, { 1.0F, 0.0F, 0.0F } },
        { { 0.5F, -0.5F, 0.0F }, { 0.0F, 1.0F, 0.0F } },
        { { 0.0F, 0.5F, 0.0F }, { 0.0F, 0.0F, 1.0F } },
    };
    m_Mesh.Indices = { 0, 1, 2 };

    m_BatchTransforms.emplace_back();
    m_BatchTransforms.back().Translate({ -0.5F, 0.5F, 0.0F });
    m_BatchTransforms.emplace_back();
    m_BatchTransforms.back().Translate({ 0.5F, 0.5F, 0.0F });

    m_Transforms.emplace_back();
    m_Transforms.back().Translate({ -0.5F, -0.5F, 0.0F });
    m_Transforms.emplace_back();
    m_Transforms.back().Translate({ 0.5F, -0.5F, 0.0F });

    NEBULA_LOG_INFO("Sandbox Layer Initializing");
}

SandboxLayer::~SandboxLayer()
{
    NEBULA_LOG_INFO("Sandbox Layer Destroyed");
}

void SandboxLayer::OnAttach()
{
    NEBULA_LOG_INFO("Sandbox Layer Attached");
}

void SandboxLayer::OnDetach()
{
    NEBULA_LOG_INFO("Sandbox Layer Detached");
}

void SandboxLayer::OnUpdate()
{
    auto& renderer = Nebula::Application::Get().GetRenderer();
    for (const auto& transform : m_BatchTransforms)
    {
        renderer.DrawMesh(m_Mesh, transform);
    }

    renderer.InstanceMesh(m_Mesh, m_Transforms);
}

void SandboxLayer::OnUIRender()
{
    ImGui::Begin("Sandbox Layer");

    ImGui::Text("FPS: %lf", 1.0 / Nebula::Time::DeltaTime());
    ImGui::Text("Batch Instances: %zu", m_BatchTransforms.size());
    ImGui::Text("Instances: %zu", m_Transforms.size());

    static std::uint32_t numInstances = 1;
    ImGui::InputScalar("Number of Instances", ImGuiDataType_U32, &numInstances);
    if (ImGui::Button("Add Instance"))
    {
        for (std::uint32_t i = 0; i < numInstances; ++i)
        {
            std::random_device random;
            std::uniform_real_distribution<float> dist(-1.0F, 1.0F);
            m_BatchTransforms.emplace_back();
            m_BatchTransforms.back().Translate({ dist(random), dist(random), 0.0F });
        }
    }
    ImGui::End();
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
    NEBULA_LOG_INFO("Sandbox Layer Event: {0}", event.ToString());
}
