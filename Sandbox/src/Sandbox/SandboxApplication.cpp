#include "SandboxApplication.hpp"

#include <utility>

#include "SandboxLayer.hpp"

SandboxApplication::SandboxApplication(Astrelis::ApplicationSpecification spec, Astrelis::CreationStatus& status) :
    Astrelis::Application(std::move(spec), status)
{
    PushLayer(static_cast<Astrelis::OwnedPtr<Astrelis::Layer*>>(Astrelis::OwnedPtr<SandboxLayer*>::Create()));
}

SandboxApplication::~SandboxApplication() { ASTRELIS_LOG_INFO("Sandbox Application Destroyed"); }
