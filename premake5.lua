workspace "Jade"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Jade/vendor/GLFW/include"

include "Jade/vendor/GLFW"

project "Jade"
    kind "SharedLib"
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
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "dwmapi.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "JADE_PLATFORM_WINDOWS",
            "JADE_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "JADE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "JADE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "JADE_DIST"
        runtime "Release"
        optimize "On"

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
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Jade/vendor/spdlog/include",
        "Jade/src"
    }

    links
    {
        "Jade"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "JADE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "JADE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "JADE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "JADE_DIST"
        optimize "On"