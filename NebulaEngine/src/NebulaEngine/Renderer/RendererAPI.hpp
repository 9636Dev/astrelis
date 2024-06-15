#pragma once

#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/VertexArray.hpp"
namespace Nebula
{
    class RendererAPI
    {
public:
    RendererAPI() = default;
    virtual ~RendererAPI() = default;
    RendererAPI(const RendererAPI &) = delete;
    RendererAPI &operator=(const RendererAPI &) = delete;
    RendererAPI(RendererAPI &&) = delete;
    RendererAPI &operator=(RendererAPI &&) = delete;
    virtual void Init() = 0;
    virtual void SetClearColor(const Vector4f &color) = 0;
    virtual void Clear() = 0;
    virtual void SetViewport(std::int32_t posX, std::int32_t posY, std::int32_t width, std::int32_t height) = 0;
    virtual void DrawIndexed(const Ptr<VertexArray> &vertexArray) = 0;

    enum class API
    {
        None = 0,
        OpenGL = 1
    };

    static Ptr<RendererAPI> Create() noexcept;
    API static GetAPI() { return s_API; }
private:
    static API s_API;
    };
} // namespace Nebula
