#include "jdpch.h"

#include "Jade/Scene/Scene.h"
#include "Jade/Scene/Components.h"
#include "Jade/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Jade
{
    Scene::Scene()
    {
        JADE_PROFILE_FUNCTION();

    }

    Scene::~Scene()
    {
    }

    entt::entity Scene::CreateEntity()
    {
        JADE_PROFILE_FUNCTION();

        return m_Registry.create();
    }

    void Scene::OnUpdate(Timestep ts)
    {
        JADE_PROFILE_FUNCTION();

        // Grouping entities with both TransformComponent and SpriteRendererComponent
        // and iterating over them for rendering
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for(entt::entity entity : group)
        {
            TransformComponent& transform = group.get<TransformComponent>(entity);
            SpriteRendererComponent& sprite = group.get<SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform.Transform, sprite.Color);
        }
    }
}