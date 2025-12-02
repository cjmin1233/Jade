#pragma once
#include <glm/glm.hpp>

namespace Jade
{
    class ImGuiUtils
    {
    public:
        ImGuiUtils() = delete;
        ~ImGuiUtils() = delete;
        ImGuiUtils(const ImGuiUtils&) = delete;
        ImGuiUtils& operator=(const ImGuiUtils&) = delete;
        ImGuiUtils(ImGuiUtils&&) = delete;
        ImGuiUtils& operator=(ImGuiUtils&&) = delete;

        static void DrawVec3Control(const std::string& label, glm::vec3& values,
            float resetValue = 0.0f, float columnWidth = 100.0f);
    };
}
