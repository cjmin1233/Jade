#pragma once
#include <entt.hpp>

#include "Jade/Core/Timestep.h"

namespace Jade
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        entt::registry& GetRegistry() { return m_Registry; }

        void OnUpdate(Timestep ts);

    private:
        entt::registry m_Registry;
    };
}