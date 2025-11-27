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
        ~Scene() = default;

        Entity CreateEntity(const std::string& name = std::string());

        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth, m_ViewportHeight;

        friend class Entity;
    };
}
