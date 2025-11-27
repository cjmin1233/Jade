#pragma once
#include "Jade/Scene/Scene.h"

#include <entt.hpp>

namespace Jade
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

#pragma region Component Methods
        // Adds a component to the entity
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)     // perfect forwarding
        {
            JADE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        // Tries to add a component, returns nullptr if already exists
        template<typename T, typename... Args>
        T* TryAddComponent(Args&&... args)  // perfect forwarding
        {
            if (HasComponent<T>())
                return nullptr;

            return &m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }
        
        // Gets a component from the entity
        template<typename T>
        T& GetComponent()
        {
            JADE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }
        template<typename T>
        const T& GetComponent() const
        {
            JADE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        // Tries to get a component, returns nullptr if not found
        template<typename T>
        T* TryGetComponent()
        {
            return m_Scene->m_Registry.try_get<T>(m_EntityHandle);
        }
        template<typename T>
        const T* TryGetComponent() const
        {
            return m_Scene->m_Registry.try_get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent() const
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        // Gets the component if it exists, otherwise adds it
        template<typename T, typename... Args>
        T& GetOrAddComponent(Args&&... args)
        {
            if (T* existing = TryGetComponent<T>())
                return *existing;

            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }
        // Removes a component from the entity
        template<typename T>
        void RemoveComponent()
        {
            JADE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        // Tries to remove a component, returns false if not found
        template<typename T>
        bool TryRemoveComponent()
        {
            if (!HasComponent<T>())
                return false;
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
            return true;
        }
#pragma endregion

        operator bool() const { return m_EntityHandle != entt::null; }

    private:
        Scene* m_Scene = nullptr;
        entt::entity m_EntityHandle = entt::null;
    };
}
