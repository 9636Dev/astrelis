#include "SandboxLayer.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Profiler.hpp"

SandboxLayer::SandboxLayer()
{
    NEBULA_LOG_INFO("Sandbox Layer Initializing");
}

SandboxLayer::~SandboxLayer()
{
    NEBULA_LOG_INFO("Sandbox Layer Destroyed");
}

void SandboxLayer::OnAttach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnAttach");
}

void SandboxLayer::OnDetach()
{
    NEBULA_PROFILE_SCOPE("SandboxLayer::OnDetach");
}

void SandboxLayer::OnUpdate()
{
}

void SandboxLayer::OnUIRender()
{
    // This is not an overlay
}

void SandboxLayer::OnEvent(Nebula::Event& event)
{
}
