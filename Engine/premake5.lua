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
		"Resources/Shaders/**.hlsl",
        "Vendor/glm/module/glm/**.hpp",
        "Vendor/glm/module/glm/**.inl",
		"Vendor/stb_image/**.cpp",
		"Vendor/stb_image/**.h"
	}

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
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
		"%{IncludeExternalDir.stb_image}"
    }

    links
    {
        "GLFW",
        "EASTL",
        "Glad",
        "opengl32.lib",
        "d3d11.lib",
		"d3dcompiler.lib",
		"dxguid.lib"
    }
	
	shadermodel("5.0")
	
	shaderassembler("AssemblyCode")
	local shader_dir = "%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}/Shader/"
	
	-- HLSL files that don't end with 'Extensions' will be ignored as they will be
    -- used as includes
	filter("files:**.hlsl")
		flags("ExcludeFromBuild")
		shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")
		shaderassembleroutput(shader_dir.."%{file.basename}"..".asm")
		
	filter("files:**_vs.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Vertex")
		shaderentry("VShader")
		
	filter("files.**_ps.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Pixel")
		shaderentry("PShader")
		
	-- Warnings as errors
	shaderoptions({"/WX"})

    filter "system:windows"
        systemversion "latest"
		
		postbuildcommands
		{
			"{COPYDIR} \"%{ShaderFilesDir.Engine}\" \"%{cfg.targetdir}\""
		}

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
