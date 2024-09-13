#pragma once

#include "Astrelis/Astrelis.hpp"

class SandboxApplication : public Astrelis::Application
{
public:
    explicit SandboxApplication(Astrelis::ApplicationSpecification spec, Astrelis::CreationStatus& status);
    ~SandboxApplication() override;
    SandboxApplication(const SandboxApplication&)            = delete;
    SandboxApplication& operator=(const SandboxApplication&) = delete;
    SandboxApplication(SandboxApplication&&)                 = delete;
    SandboxApplication& operator=(SandboxApplication&&)      = delete;
};
