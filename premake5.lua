include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Jade"
    architecture "x86_64"
    startproject "Jade-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile",
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Jade/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Jade/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Jade/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Jade/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Jade/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Jade/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Jade/vendor/yaml-cpp/include"

group "Dependencies"
    include "vendor/premake"
    include "Jade/vendor/GLFW"
    include "Jade/vendor/Glad"
    include "Jade/vendor/imgui"
    include "Jade/vendor/yaml-cpp"
group ""

include "Jade"

include "Sandbox"

include "Jade-Editor"
