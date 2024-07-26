#include "SandboxApplication.hpp"

#include <utility>

#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Application.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

SandboxApplication::SandboxApplication(Nebula::ApplicationSpecification spec, Nebula::CreationStatus& status) :
    Nebula::Application(std::move(spec), status)
{
    NEBULA_PROFILE_SCOPE("SandboxApplication::SandboxApplication");
    PushLayer(static_cast<Nebula::OwnedPtr<Nebula::Layer*>>(Nebula::OwnedPtr<SandboxLayer*>::Create()));
}

SandboxApplication::~SandboxApplication() { NEBULA_LOG_INFO("Sandbox Application Destroyed"); }
