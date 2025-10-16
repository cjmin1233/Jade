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

project "Jade"
    location "Jade"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "jdpch.h"
    pchsource "Jade/src/jdpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:windows"
        staticruntime "On"
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
        symbols "On"

    filter "configurations:Release"
        defines "JADE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "JADE_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

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
        staticruntime "On"
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