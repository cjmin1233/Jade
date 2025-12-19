include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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
