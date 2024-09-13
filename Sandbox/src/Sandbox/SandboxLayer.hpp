#pragma once

#include "Astrelis/Astrelis.hpp"
#include "Astrelis/IO/Image.hpp"

#include <future>

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
    Astrelis::Milliseconds m_CpuTime;

    std::future<Astrelis::InMemoryImage> m_ImageFuture;
};
