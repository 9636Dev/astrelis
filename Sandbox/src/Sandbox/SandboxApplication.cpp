#include "SandboxApplication.hpp"

#include <utility>

SandboxApplication::SandboxApplication(Nebula::ApplicationSpecification spec)
    : Nebula::Application(std::move(spec))
{
}

SandboxApplication::~SandboxApplication() = default;
