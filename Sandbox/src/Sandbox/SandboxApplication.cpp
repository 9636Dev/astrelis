#include "SandboxApplication.hpp"
#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Profiler.hpp"

#include <utility>

SandboxApplication::SandboxApplication(Nebula::ApplicationSpecification spec) : Nebula::Application(std::move(spec))
{
    NEBULA_PROFILE_SCOPE("SandboxApplication::SandboxApplication");
    PushLayer(Nebula::OwnedPtr<SandboxLayer*>::Create());
}

SandboxApplication::~SandboxApplication() { NEBULA_LOG_INFO("Sandbox Application Destroyed"); }
