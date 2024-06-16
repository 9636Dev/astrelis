#pragma once

#include "NebulaEngine/Core/Layer.hpp"

class EditorLayer : public Nebula::Layer
{
public:
    EditorLayer();
    ~EditorLayer() override;
    EditorLayer(const EditorLayer& other) = delete;
    EditorLayer& operator=(const EditorLayer& other) = delete;
    EditorLayer(EditorLayer&& other) = delete;
    EditorLayer& operator=(EditorLayer&& other) = delete;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnUIRender() override;
    void OnEvent(Nebula::Event& event) override;
};
