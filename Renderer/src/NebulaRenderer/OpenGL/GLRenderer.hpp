#pragma once


#include <GL/glew.h>
#include "../Renderer.hpp"

namespace Nebula
{
    class GLRenderer : public Renderer
    {
    public:
        explicit GLRenderer(Ref<Window> window) noexcept;
        ~GLRenderer() override;
        GLRenderer(const GLRenderer&)            = delete;
        GLRenderer(GLRenderer&&)                 = delete;
        GLRenderer& operator=(const GLRenderer&) = delete;
        GLRenderer& operator=(GLRenderer&&)      = delete;

        void SetClearColor(float red, float green, float blue, float alpha) override;
        void NewFrame() override;
        void Render() override;
    private:
        Ref<Window> m_Window;

        GLuint m_VertexArray;
        GLuint m_VertexBuffer;
        GLuint m_IndexBuffer;
        GLuint m_ShaderProgram;
        GLuint m_FrameBuffer;
        GLuint m_Texture;
        GLuint m_RenderBuffer;
    };
} // namespace Nebula
