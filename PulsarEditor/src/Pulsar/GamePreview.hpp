#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Renderer/Camera.hpp"
#include "NebulaEngine/Renderer/Renderer2D.hpp"
#include "NebulaEngine/Scene/Scene2D.hpp"
namespace Pulsar
{
    class GamePreview2D
    {
    public:
        GamePreview2D(Nebula::RefPtr<Nebula::Window> window, Nebula::Bounds viewport);
        ~GamePreview2D();
        GamePreview2D(const GamePreview2D&) = delete;
        GamePreview2D& operator=(const GamePreview2D&) = delete;
        GamePreview2D(GamePreview2D&&) noexcept      = delete;
        GamePreview2D& operator=(GamePreview2D&&) noexcept = delete;

        void RenderScene(Nebula::Scene2D& scene);
        void Render();

    private:
        Nebula::Renderer2D m_Renderer;
        Nebula::Camera m_Camera;

        Nebula::Bounds m_Viewport;
    };
} // namespace Pulsar
