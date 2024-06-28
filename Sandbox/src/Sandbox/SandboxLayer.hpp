#pragma once

#include "NebulaEngine/Core/Layer.hpp"
#include "NebulaEngine/Core/Time.hpp"
#include "NebulaEngine/Events/WindowEvent.hpp"
#include "NebulaEngine/Renderer/Camera.hpp"
#include "NebulaEngine/Renderer/VoxelRenderer.hpp"
#include "NebulaEngine/Scene/VoxelScene.hpp"

class SandboxLayer : public Nebula::Layer
{
public:
    SandboxLayer();
    ~SandboxLayer() override;
    SandboxLayer(const SandboxLayer&)            = delete;
    SandboxLayer& operator=(const SandboxLayer&) = delete;
    SandboxLayer(SandboxLayer&&)                 = delete;
    SandboxLayer& operator=(SandboxLayer&&)      = delete;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnUIRender() override;
    void OnEvent(Nebula::Event& event) override;

    void OnViewportResize(Nebula::WindowResizedEvent& event);
private:
    Nebula::Camera m_Camera;
    Nebula::VoxelScene m_Scene;
    Nebula::ScopedPtr<Nebula::VoxelRenderer> m_Renderer = nullptr;

    Nebula::Milliseconds m_CpuTime;
};
