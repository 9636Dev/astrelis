#include "SandboxApplication.hpp"
#include "SandboxLayer.hpp"

#include <utility>

SandboxApplication::SandboxApplication(Nebula::ApplicationSpecification spec) : Nebula::Application(std::move(spec))
{
    NEBULA_LOG_INFO("Sandbox Application Initializing");

    gsl::owner<Nebula::Layer*> layer = new SandboxLayer();
    PushOverlay(layer);
    layer = nullptr;
}

SandboxApplication::~SandboxApplication() { NEBULA_LOG_INFO("Sandbox Application Destroyed"); }
