#pragma once

#include "Astrelis/Core/Layer.hpp"
#include "Astrelis/Renderer/Renderer2D.hpp"

#include <string>

#include "AssetPanel.hpp"
#include "Console.hpp"

namespace AstrelisEditor {
    class EditorLayer : public Astrelis::Layer {
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
        Console    m_Console;
        AssetPanel m_AssetPanel;

        Astrelis::Renderer2D m_Renderer2D;
    };
} // namespace AstrelisEditor
