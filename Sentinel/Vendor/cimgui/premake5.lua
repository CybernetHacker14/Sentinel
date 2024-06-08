project "cimgui"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "module/cimgui.h",
        "module/generator/cimgui_impl.h",
        "module/cimgui.cpp",
        "module/imgui/imconfig.h",
        "module/imgui/imgui.h",
        "module/imgui/imgui.cpp",
        "module/imgui/imgui_draw.cpp",
        "module/imgui/imgui_internal.h",
        "module/imgui/imgui_tables.cpp",
        "module/imgui/imgui_widgets.cpp",
        "module/imgui/imstb_rectpack.h",
        "module/imgui/imstb_textedit.h",
        "module/imgui/imstb_truetype.h",
        "module/imgui/imgui_demo.cpp"
    }

    includedirs
    {
        "module/imgui",
        "%{VendorIncludes.glfw}"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"