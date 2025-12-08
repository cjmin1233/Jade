#pragma once
#include "Jade/Core/Timestep.h"
#include "Jade/Renderer/Camera/EditorCamera.h"

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
        Entity CreateEntity(const std::string& name, uint32_t id);
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(Timestep ts);
        void OnUpdateEditor(Timestep ts, EditorCamera& camera);
        void OnViewportResize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        const std::string& GetFilePath() const { return m_FilePath; }
        void SetFilePath(const std::string& path) { m_FilePath = path; }

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth, m_ViewportHeight;
        std::string m_Name;
        std::string m_FilePath;

        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;
    };
}
