#pragma once


#include <GL/glew.h>
#include "../Renderer.hpp"

namespace Nebula
{
    class GLRenderer : public Renderer
    {
    public:
        explicit GLRenderer(const RenderingContext& context) noexcept;
        ~GLRenderer() override;
        GLRenderer(const GLRenderer&)            = delete;
        GLRenderer(GLRenderer&&)                 = delete;
        GLRenderer& operator=(const GLRenderer&) = delete;
        GLRenderer& operator=(GLRenderer&&)      = delete;

        void Resize(Dimension dimensions) override;

        void SetClearColor(float red, float green, float blue, float alpha) override;
        void NewFrame() override;
        void Render() override;
    private:
        Ref<Window> m_Window;
        Dimension m_Dimensions;

        GLuint m_VertexArray;
        GLuint m_VertexBuffer;
        GLuint m_IndexBuffer;
        GLuint m_ShaderProgram;

        GLuint m_FrameBuffer;
        GLuint m_Texture;
        GLuint m_RenderBuffer;
    };
} // namespace Nebula
