project "Sentinel-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{IncludeInternalDir.Sentinel_Source}",
        "%{IncludeExternalDir.glm}",
        "%{IncludeExternalDir.EASTL}",
		"%{IncludeExternalDir.EABase}",
        "%{IncludeExternalDir.spdlog}",
		"%{IncludeExternalDir.cereal}"
    }

    links
    {
        "Sentinel"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "ST_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ST_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "ST_DIST"
        runtime "Release"
        optimize "on"