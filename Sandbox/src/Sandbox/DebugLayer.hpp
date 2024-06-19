#pragma once

#include "NebulaEngine/Core/Layer.hpp"

class DebugLayer : public Nebula::Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate() override {}
    void OnUIRender() override;
    void OnEvent(Nebula::Event& event) override;
};
