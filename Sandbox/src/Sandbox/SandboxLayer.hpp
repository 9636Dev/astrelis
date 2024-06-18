#pragma once

#include "NebulaEngine/Core/Layer.hpp"
#include "NebulaEngine/Events/WindowEvent.hpp"
#include "NebulaEngine/Renderer/Renderer2D.hpp"

class SandboxLayer : public Nebula::Layer
{
public:
    SandboxLayer();
    ~SandboxLayer() override;
    SandboxLayer(const SandboxLayer&) = delete;
    SandboxLayer& operator=(const SandboxLayer&) = delete;
    SandboxLayer(SandboxLayer&&) = delete;
    SandboxLayer& operator=(SandboxLayer&&) = delete;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnUIRender() override;
    void OnEvent(Nebula::Event& event) override;

    void OnViewportResize(Nebula::WindowResizedEvent& event);
private:
    Nebula::ScopedPtr<Nebula::Renderer2D> m_Renderer2D = nullptr;
};
