project "Sandbox"
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

    includedirs
    {
        "%{IncludeDir.Sentinel_Source}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.EASTL}",
        "%{IncludeDir.EABase}",
        "%{IncludeDir.spdlog}"
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
