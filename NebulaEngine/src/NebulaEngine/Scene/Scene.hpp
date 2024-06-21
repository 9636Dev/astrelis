#pragma once

#include <entt/entt.hpp>

namespace Nebula
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        Scene(const Scene& other)            = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene(Scene&& other)                 = delete;
        Scene& operator=(Scene&& other)      = delete;
    private:
        entt::registry m_Registry;
    };
} // namespace Nebula
