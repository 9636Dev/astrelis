#pragma once

#include "Astrelis/Core/Geometry.hpp"
#include "Astrelis/Renderer/Camera.hpp"
#include "Astrelis/Renderer/Renderer2D.hpp"
#include "Astrelis/Scene/Scene2D.hpp"
namespace Pulsar
{
    class GamePreview2D
    {
    public:
        GamePreview2D(Astrelis::RefPtr<Astrelis::Window> window, Astrelis::Rect2Di viewport);
        ~GamePreview2D();
        GamePreview2D(const GamePreview2D&) = delete;
        GamePreview2D& operator=(const GamePreview2D&) = delete;
        GamePreview2D(GamePreview2D&&) noexcept      = delete;
        GamePreview2D& operator=(GamePreview2D&&) noexcept = delete;

        void RenderScene(Astrelis::Scene2D& scene);
        void Render();

    private:
        Astrelis::Renderer2D m_Renderer;
        Astrelis::Camera m_Camera;

        Astrelis::Rect2Di m_Viewport;
    };
} // namespace Pulsar
