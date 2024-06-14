#pragma once

namespace Nebula
{
    class Entity
    {
    public:
        Entity() = default;
        ~Entity() = default;
        Entity(const Entity &) = default;
        Entity &operator=(const Entity &) = default;
        Entity(Entity &&) = default;
        Entity &operator=(Entity &&) = default;
    };
} // namespace Nebula
