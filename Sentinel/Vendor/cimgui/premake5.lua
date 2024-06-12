project "cimgui"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "module/cimgui.cpp",
        "module/imgui/imgui.cpp",
        "module/imgui/imgui_draw.cpp",
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

        files
        {
            "module/imgui/backends/imgui_impl_glfw.cpp",
            "module/imgui/backends/imgui_impl_dx11.cpp"
        }

        defines
        {
            "IMGUI_IMPL_API=extern \"C\" __declspec(dllexport)"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"