#include "SandboxApplication.hpp"
#include "DebugLayer.hpp"
#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Profiler.hpp"

#include <utility>

SandboxApplication::SandboxApplication(Nebula::ApplicationSpecification spec, Nebula::CreationStatus& status) :
    Nebula::Application(std::move(spec), status)
{
    NEBULA_PROFILE_SCOPE("SandboxApplication::SandboxApplication");
    PushLayer(static_cast<Nebula::OwnedPtr<Nebula::Layer*>>(Nebula::OwnedPtr<SandboxLayer*>::Create()));
    PushOverlay(static_cast<Nebula::OwnedPtr<Nebula::Layer*>>(Nebula::OwnedPtr<DebugLayer*>::Create()));
}

SandboxApplication::~SandboxApplication() { NEBULA_LOG_INFO("Sandbox Application Destroyed"); }
