#pragma once

#include <vector>
#include "Entity.hpp"

namespace Nebula
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        Scene(const Scene &) = delete;
        Scene &operator=(const Scene &) = delete;
        Scene(Scene &&) = delete;
        Scene &operator=(Scene &&) = delete;


    private:
        std::vector<Entity> m_Entities;
    };
} // namespace Nebula
