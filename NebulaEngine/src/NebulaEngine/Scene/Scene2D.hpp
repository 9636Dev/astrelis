#pragma once

#include <entt/entt.hpp>

namespace Nebula
{
    using Entity = entt::entity;

    class Scene2D
    {
    public:
        Scene2D() = default;

        Entity CreateEntity() { return m_Registry.create(); }

        void DestroyEntity(Entity entity) { m_Registry.destroy(entity); }

        bool IsDirty() const { return m_Dirty; }

        void SetDirty(bool dirty) { m_Dirty = dirty; }

        template<typename T> void AddComponent(Entity entity, T component) { m_Registry.emplace<T>(entity, component); }

        template<typename T> void RemoveComponent(Entity entity) { m_Registry.remove<T>(entity); }

        template<typename T> T& GetComponent(Entity entity) { return m_Registry.get<T>(entity); }

        template<typename T> auto GetComponents() { return m_Registry.view<T>(); }
    private:
        entt::registry m_Registry;
        bool m_Dirty = false;
    };
} // namespace Nebula
