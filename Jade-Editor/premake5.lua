project "Jade-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/Jade/vendor/spdlog/include",
        "%{wks.location}/Jade/src",
        "%{wks.location}/Jade/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}",
    }

    links
    {
        "Jade",
    }

    filter "system:windows"
        systemversion "latest"

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
