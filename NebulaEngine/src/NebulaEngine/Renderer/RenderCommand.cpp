#include "RenderCommand.hpp"


namespace Nebula
{
    Ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

    void RenderCommand::Init()
    {
        s_RendererAPI->Init();
    }

    void RenderCommand::SetClearColor(const Vector4f &color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        s_RendererAPI->Clear();
    }

    void RenderCommand::SetViewport(std::int32_t posX, std::int32_t posY, std::int32_t width, std::int32_t height)
    {
        s_RendererAPI->SetViewport(posX, posY, width, height);
    }

    void RenderCommand::DrawIndexed(const Ptr<VertexArray> &vertexArray)
    {
        s_RendererAPI->DrawIndexed(vertexArray);
    }
} // namespace Nebula
