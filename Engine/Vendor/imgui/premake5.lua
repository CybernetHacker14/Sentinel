project "imgui"
	kind "StaticLib"
	language "C++"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"module/imconfig.h",
		"module/imgui.h",
		"module/imgui.cpp",
		"module/imgui_draw.cpp",
		"module/imgui_internal.h",
		"module/imgui_tables.cpp",
		"module/imgui_widgets.cpp",
		"module/imstb_rectpack.h",
		"module/imstb_textedit.h",
		"module/imstb_truetype.h",
		"module/imgui_demo.cpp"
	}
	
	filter "system:windows"
        systemversion "latest"
        cppdialect "C++11"
        staticruntime "On"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++11"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"