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
        "Source/Sentinel.h",
        "Source/stpch.h",
        "Source/stpch.cpp",
		"Source/Sentinel/**.h",
        "Source/Sentinel/**.cpp",
		"Resources/**.inl",
        "Vendor/glm/module/glm/**.hpp",
        "Vendor/glm/module/glm/**.inl",
		"Vendor/stb_image/**.cpp",
		"Vendor/stb_image/**.h"
	}

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "_DISABLE_VECTOR_ANNOTATION",
        "_DISABLE_STRING_ANNOTATION",
        "FLECS_STATIC"
    }

    includedirs
    {
        "Source",
		"%{IncludeExternalDir.EASTL}",
		"%{IncludeExternalDir.EABase}",
        "%{IncludeExternalDir.GLFW}",
        "%{IncludeExternalDir.Glad}",
        "%{IncludeExternalDir.glm}",
        "%{IncludeExternalDir.spdlog}",
		"%{IncludeExternalDir.stb_image}",
		"%{IncludeExternalDir.cereal}",
		"%{IncludeExternalDir.imgui}",
        "%{IncludeExternalDir.flecs}",
        "%{IncludeExternalDir.minizip}"
    }

    links
    {
        "GLFW",
        "EASTL",
		"opengl32.lib",
        "d3d11.lib",
		"shlwapi.lib",
		"d3dcompiler.lib",
		"dxguid.lib",
		"dwmapi.lib",
        "bcrypt.lib",
		"imgui",
        "flecs",
        "minizip"
    }
		
    filter "system:windows"
        systemversion "latest"
		
		postbuildcommands
		{
			"{COPYDIR} \"%{ShaderFilesDir.Engine}\" \"%{cfg.targetdir}\""
		}

        files {
            "Source/Platform/Windows/**.cpp",
            "Source/Platform/Windows/**.h"
        }

    filter { "options:gfxapi=DX11" }
        defines "ST_RENDERER_DX11"

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
