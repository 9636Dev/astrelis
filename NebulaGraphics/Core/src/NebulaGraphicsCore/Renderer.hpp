#pragma once

namespace Nebula
{
    class Renderer
    {
    private:
        Renderer() = default;
    public:
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        virtual ~Renderer() = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
    };
} // namespace Nebula
