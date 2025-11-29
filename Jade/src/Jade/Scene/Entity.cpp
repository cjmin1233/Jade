#include "jdpch.h"

#include "Jade/Scene/Entity.h"

namespace Jade
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle)
        , m_Scene(scene)
    {
    }
}
