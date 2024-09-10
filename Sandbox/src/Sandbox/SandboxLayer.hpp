#pragma once

#include <future>

#include "Astrelis/Core/Layer.hpp"
#include "Astrelis/Core/Time.hpp"
#include "Astrelis/Events/WindowEvent.hpp"
#include "Astrelis/IO/Image.hpp"
#include "Astrelis/Renderer/Camera.hpp"
#include "Astrelis/Renderer/Renderer2D.hpp"
#include "Astrelis/Scene/Scene2D.hpp"

class SandboxLayer : public Astrelis::Layer
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
    void OnEvent(Astrelis::Event& event) override;

    void OnViewportResize(Astrelis::WindowResizedEvent& event);
private:
    Astrelis::Camera m_Camera;
    Astrelis::Scene2D m_Scene;
    Astrelis::ScopedPtr<Astrelis::Renderer2D> m_Renderer = nullptr;

    Astrelis::Milliseconds m_CpuTime;

    std::future<Astrelis::InMemoryImage> m_ImageCapture;
};
