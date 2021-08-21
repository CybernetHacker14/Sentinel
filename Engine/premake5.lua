project "Sentinel"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "stpch.h"
    pchsource "Source/stpch.cpp"

	files
	{
		"Source/**.h",
        "Source/**.cpp",
        "Vendor/glm/module/glm/**.hpp",
        "Vendor/glm/module/glm/**.inl",
	}

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
    }

    includedirs
    {
        "Source",
        "%{IncludeExternalDir.GLFW}",
        "%{IncludeExternalDir.Glad}",
        "%{IncludeExternalDir.glm}",
        "%{IncludeExternalDir.EABase}",
        "%{IncludeExternalDir.EASTL}",
        "%{IncludeExternalDir.spdlog}"
    }

    links
    {
        "GLFW",
        "EASTL",
        "Glad",
        "opengl32.lib",
        "d3d11.lib",
		"d3dcompiler.lib"
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
