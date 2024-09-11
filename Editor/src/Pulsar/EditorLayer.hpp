#pragma once

#include "Astrelis/Core/Layer.hpp"

#include "AssetPanel.hpp"
#include "Astrelis/Scene/Scene2D.hpp"
#include "Pulsar/Console.hpp"
#include "Pulsar/GamePreview.hpp"
#include <string>

namespace Pulsar
{
    class EditorLayer : public Astrelis::Layer
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
        void OnEvent(Astrelis::Event& event) override;
    private:
        Console m_Console;
        AssetPanel m_AssetPanel;
        GamePreview2D m_GamePreview;
    };
} // namespace Pulsar
