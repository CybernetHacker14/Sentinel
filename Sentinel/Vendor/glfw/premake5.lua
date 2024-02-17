project "glfw"
    kind "StaticLib"
    language "C"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "module/include/GLFW/glfw3.h",
        "module/include/GLFW/glfw3native.h",
        "module/src/glfw_config.h",
        "module/src/internal.h",
        "module/src/platform.h",
        "module/src/mappings.h",
        "module/src/platform.c",
        "module/src/context.c",
        "module/src/init.c",
        "module/src/input.c",
        "module/src/monitor.c",
        "module/src/vulkan.c",
        "module/src/window.c",
		"module/src/null_init.c",
		"module/src/null_joystick.c",
		"module/src/null_monitor.c",
		"module/src/null_platform.h",
		"module/src/null_window.c"
    }

    filter "system:linux"
        pic "On"

        systemversion "latest"
        staticruntime "On"

        files
        {
            "module/src/x11_init.c",
            "module/src/x11_monitor.c",
            "module/src/x11_window.c",
            "module/src/xkb_unicode.c",
            "module/src/posix_time.c",
            "module/src/posix_thread.c",
            "module/src/glx_context.c",
            "module/src/egl_context.c",
            "module/src/osmesa_context.c",
            "module/src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        files
        {
			"module/src/win32_init.c",
            "module/src/win32_module.c",
            "module/src/win32_joystick.c",
            "module/src/win32_monitor.c",
            "module/src/win32_time.h",
            "module/src/win32_time.c",
            "module/src/win32_thread.h",
            "module/src/win32_thread.c",
            "module/src/win32_window.c",
            "module/src/wgl_context.c",
            "module/src/egl_context.c",
            "module/src/osmesa_context.c"
        }

        defines
        {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
            "WIN32_LEAN_AND_MEAN"
        }

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"