#pragma once

#include "../Renderer.hpp"


namespace Nebula
{
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer(const OpenGLRenderer&)            = delete;
        OpenGLRenderer(OpenGLRenderer&&)                 = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&)      = delete;
        explicit OpenGLRenderer(Ref<Window> window);
        ~OpenGLRenderer() override;

        void Clear() override;

    private:
        Ref<Window> m_Window;
    };
} // namespace Nebula
