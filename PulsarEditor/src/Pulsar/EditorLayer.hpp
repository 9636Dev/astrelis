#pragma once

#include "NebulaEngine/Core/Layer.hpp"

#include "AssetPanel.hpp"
#include "NebulaEngine/Scene/Scene2D.hpp"
#include "Pulsar/Console.hpp"
#include "Pulsar/GamePreview.hpp"
#include <string>

namespace Pulsar
{
    class EditorLayer : public Nebula::Layer
    {
    public:
        explicit EditorLayer(std::string rootDirectory);
        ~EditorLayer() override;
        EditorLayer(const EditorLayer& other)            = delete;
        EditorLayer& operator=(const EditorLayer& other) = delete;
        EditorLayer(EditorLayer&& other)                 = delete;
        EditorLayer& operator=(EditorLayer&& other)      = delete;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnUIRender() override;
        void OnEvent(Nebula::Event& event) override;
    private:
        Console m_Console;
        AssetPanel m_AssetPanel;
        GamePreview2D m_GamePreview;
        Nebula::Scene2D m_Scene;
    };
} // namespace Pulsar
