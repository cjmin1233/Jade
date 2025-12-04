#include "jdpch.h"

#include "Jade/Scene/SceneSerializer.h"
#include "Jade/Scene/Entity.h"
#include "Jade/Scene/Components.h"

#include <fstream>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    // GLM Vector3 and Vector4 serialization
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace Jade
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << (uint64_t)(uint32_t)entity;

#pragma region TagComponent Serialization
        if (TagComponent* tagComponent = entity.TryGetComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            // TagComponent properties
            out << YAML::Key << "Tag" << YAML::Value << tagComponent->Tag;

            out << YAML::EndMap;
        }
#pragma endregion

#pragma region TransformComponent Serialization
        if (TransformComponent* transformComponent = entity.TryGetComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent" << YAML::Value;
            out << YAML::BeginMap;

            // TransformComponent properties
            out << YAML::Key << "Translation" << YAML::Value << transformComponent->Translation;
            out << YAML::Key << "Rotation" << YAML::Value << transformComponent->Rotation;
            out << YAML::Key << "Scale" << YAML::Value << transformComponent->Scale;

            out << YAML::EndMap;
        }
#pragma endregion

#pragma region CameraComponent Serialization
        if (CameraComponent* cameraComponent = entity.TryGetComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent" << YAML::Value;
            out << YAML::BeginMap;

            // SceneCamera properties
            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)cameraComponent->Cam.GetProjectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << cameraComponent->Cam.GetPerspectiveFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << cameraComponent->Cam.GetPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << cameraComponent->Cam.GetPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << cameraComponent->Cam.GetOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << cameraComponent->Cam.GetOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << cameraComponent->Cam.GetOrthographicFarClip();
            out << YAML::EndMap;

            // CameraComponent properties
            out << YAML::Key << "Primary" << YAML::Value << cameraComponent->Primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent->FixedAspectRatio;

            out << YAML::EndMap;
        }
#pragma endregion

#pragma region SpriteRendererComponent Serialization
        if (SpriteRendererComponent* spriteRendererComponent = entity.TryGetComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent" << YAML::Value;
            out << YAML::BeginMap;

            // SpriteRendererComponent properties
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent->Color;

            out << YAML::EndMap;
        }
#pragma endregion

        out << YAML::EndMap; // Entity
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;

        // Scene name serialization
        out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();

        // Entities serialization
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_Scene->m_Registry.view<entt::entity>().each([&](entt::entity entityID)
            {
                Entity entity{ entityID, m_Scene.get() };

                if (!entity)
                    return;

                SerializeEntity(out, entity);
            });

        out << YAML::EndSeq;

        out << YAML::EndMap;

        const std::string sceneExtension = ".jade";
        std::ofstream fout(filepath + m_Scene->GetName() + sceneExtension);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        // 
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
            return false;

        // Scene name deserialization
        std::string sceneName = data["Scene"].as<std::string>();
        m_Scene->SetName(sceneName);
        JADE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        // Entities deserialization
        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                // Entity deserialization
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                // TagComponent deserialization
                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                {
                    name = tagComponent["Tag"].as<std::string>();
                }

                JADE_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

                Entity deserializedEntity = m_Scene->CreateEntity(name);

#pragma region TransformComponent Deserialization
                // TransformComponent deserialization
                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();

                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }
#pragma endregion

#pragma region CameraComponent Deserialization
                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();
                    const auto& cameraProps = cameraComponent["Camera"];

                    // Camera properties
                    cc.Cam.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

                    cc.Cam.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
                    cc.Cam.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    cc.Cam.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                    cc.Cam.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    cc.Cam.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    cc.Cam.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }
#pragma endregion

#pragma region SpriteRendererComponent Deserialization
                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
                }
#pragma endregion
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        // 
        return false;
    }
}
