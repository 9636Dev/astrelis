#pragma once

#include <NebulaGraphicsCore/Renderer.hpp>
#include <NebulaGraphicsCore/Window.hpp>

namespace Nebula
{
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override;
        OpenGLRenderer(const OpenGLRenderer&) = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&) = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

        void Render() override;

    private:
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::size_t s_RendererCount = 0;
    };
} // namespace Nebula
