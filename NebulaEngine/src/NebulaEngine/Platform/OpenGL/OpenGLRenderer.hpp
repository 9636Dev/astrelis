#pragma once

#include "NebulaEngine/Renderer/Renderer.hpp"

namespace Nebula
{
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer(Ref<Window> window, Bounds bounds);
        ~OpenGLRenderer() override;
        OpenGLRenderer(const OpenGLRenderer&)            = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&)                 = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&)      = delete;

        void Clear() override;

        static Result<Ptr<OpenGLRenderer>, std::string> Create(Ref<Window> window, Bounds bounds);
    private:
        Bounds m_Bounds;
    };
} // namespace Nebula
