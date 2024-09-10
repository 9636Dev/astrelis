#pragma once

#include "Astrelis/Renderer/RendererAPI.hpp"

namespace Astrelis
{
    class OpenGLRendererAPI : public RendererAPI
    {
public:
        OpenGLRendererAPI() noexcept = default;
        ~OpenGLRendererAPI() override = default;
        OpenGLRendererAPI(const OpenGLRendererAPI &) = delete;
        OpenGLRendererAPI &operator=(const OpenGLRendererAPI &) = delete;
        OpenGLRendererAPI(OpenGLRendererAPI &&) = delete;
        OpenGLRendererAPI &operator=(OpenGLRendererAPI &&) = delete;

        void Init() override;
        void SetClearColor(const Vector4f &color) override;
        void Clear() override;
        void SetViewport(std::int32_t posX, std::int32_t posY, std::int32_t width, std::int32_t height) override;
        void DrawIndexed(const Ptr<VertexArray> &vertexArray) override;
    };
} // namespace Astrelis
