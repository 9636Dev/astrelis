#pragma once

#include "NebulaEngine/Core/Application.hpp"

class SandboxApplication : public Nebula::Application
{
public:
    explicit SandboxApplication(Nebula::ApplicationSpecification spec, Nebula::CreationStatus& status);
    ~SandboxApplication() override;
    SandboxApplication(const SandboxApplication&)            = delete;
    SandboxApplication& operator=(const SandboxApplication&) = delete;
    SandboxApplication(SandboxApplication&&)                 = delete;
    SandboxApplication& operator=(SandboxApplication&&)      = delete;
};
