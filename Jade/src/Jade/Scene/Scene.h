#pragma once
#include "Jade/Core/Timestep.h"

#include <entt.hpp>

namespace Jade
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        Scene(const std::string& name);
        ~Scene() = default;

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

        std::string& GetName() { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth, m_ViewportHeight;
        std::string m_Name;

        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;
    };
}
