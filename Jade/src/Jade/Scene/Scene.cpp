#include "jdpch.h"

#include "Jade/Scene/Scene.h"
#include "Jade/Scene/Components.h"
#include "Jade/Renderer/Renderer2D.h"
#include "Jade/Scene/Entity.h"

#include <glm/glm.hpp>

namespace Jade
{
    Scene::Scene()
        : m_Registry()
        , m_ViewportWidth(1600)
        , m_ViewportHeight(900)
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        JADE_PROFILE_FUNCTION();

        entt::entity entityHandle = m_Registry.create();
        Entity entity{ entityHandle, this };
        // Every entity gets a TransformComponent by default
        entity.AddComponent<TransformComponent>();

        TagComponent& tag = entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);

        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        JADE_PROFILE_FUNCTION();

        {
            // Update Scripts
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    // If there is no instance, instantiate the script
                    if (!nsc.Instance)
                    {
                        nsc.Instance = nsc.InstantiateScript();
                        nsc.Instance->m_Entity = Entity{ entity, this };

                        nsc.Instance->OnCreate();
                    }

                    // Call the script's OnUpdate method
                    nsc.Instance->OnUpdate(ts);
                });
        }

        CameraComponent* mainCamera = nullptr;
        TransformComponent* cameraTransform = nullptr;

        auto view = m_Registry.view<TransformComponent, CameraComponent>();

        // Find the primary camera in the scene
        for (auto [entity, transform, camera] : view.each())
        {
            if (camera.Primary)
            {
                mainCamera = &camera;
                cameraTransform = &transform;
                break;
            }
        }

        // If a primary camera is found, begin the scene rendering
        if (mainCamera && cameraTransform)
        {
            Renderer2D::BeginScene(mainCamera->Cam, cameraTransform->GetTransform());

            // Grouping entities with both TransformComponent and SpriteRendererComponent
            // and iterating over them for rendering
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (entt::entity entity : group)
            {
                TransformComponent& transform = group.get<TransformComponent>(entity);
                SpriteRendererComponent& sprite = group.get<SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize all non-fixed aspect ratio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Cam.SetViewportSize(width, height);
            }
        }
    }
}
