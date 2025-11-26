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

    private:
        entt::registry m_Registry;

        friend class Entity;
    };
}