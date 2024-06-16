#include "SandboxApplication.hpp"
#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Profiler.hpp"

#include <utility>

SandboxApplication::SandboxApplication(Nebula::ApplicationSpecification spec) : Nebula::Application(std::move(spec))
{
    NEBULA_PROFILE_SCOPE("SandboxApplication::SandboxApplication");
    gsl::owner<Nebula::Layer*> layer = new SandboxLayer();
    PushLayer(layer);
    layer = nullptr;
}

SandboxApplication::~SandboxApplication() { NEBULA_LOG_INFO("Sandbox Application Destroyed"); }
