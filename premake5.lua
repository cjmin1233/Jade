workspace "Jade"
    architecture "x86_64"
    startproject "Sandbox"

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
IncludeDir["GLFW"] = "Jade/vendor/GLFW/include"
IncludeDir["Glad"] = "Jade/vendor/Glad/include"
IncludeDir["ImGui"] = "Jade/vendor/imgui"
IncludeDir["glm"] = "Jade/vendor/glm"
IncludeDir["stb_image"] = "Jade/vendor/stb_image"

group "Dependencies"
    include "Jade/vendor/GLFW"
    include "Jade/vendor/Glad"
    include "Jade/vendor/imgui"
group ""

project "Jade"
    kind "StaticLib"
    location "Jade"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "jdpch.h"
    pchsource "%{prj.name}/src/jdpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"    
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",  
        "%{IncludeDir.stb_image}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "JADE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

    filter "configurations:Debug"
        defines "JADE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "JADE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "JADE_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "Jade/vendor/spdlog/include",
        "Jade/src",
        "Jade/vendor",
        "%{IncludeDir.glm}",
    }

    links
    {
        "Jade",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "JADE_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "JADE_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "JADE_DIST"
        optimize "on"