#include "SandboxApplication.hpp"

#include <utility>

#include "SandboxLayer.hpp"

#include "Astrelis/Core/Application.hpp"
#include "Astrelis/Core/Log.hpp"
#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Profiler.hpp"

SandboxApplication::SandboxApplication(Astrelis::ApplicationSpecification spec, Astrelis::CreationStatus& status) :
    Astrelis::Application(std::move(spec), status)
{
    ASTRELIS_PROFILE_SCOPE("SandboxApplication::SandboxApplication");
    PushLayer(static_cast<Astrelis::OwnedPtr<Astrelis::Layer*>>(Astrelis::OwnedPtr<SandboxLayer*>::Create()));
}

SandboxApplication::~SandboxApplication() { ASTRELIS_LOG_INFO("Sandbox Application Destroyed"); }
