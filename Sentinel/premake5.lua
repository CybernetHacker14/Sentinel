project "Sentinel"
    kind "SharedLib"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "stpch.h"
    pchsource "Source/Runtime/stpch.c"

    files
    {
        "Source/Runtime/**.h",
        "Source/Runtime/**.c",
    }

    includedirs
    {
        "Source/Runtime",
        "%{VendorIncludes.glfw}"
    }

    implibdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")

    links
    {
        "glfw"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "Source/Platform/Windows/**.h",
            "Source/Platform/Windows/**.c"
        }

        includedirs
        {
            "Source/Platform/Windows"
        }

        defines
        {
            "WIN32_LEAN_AND_MEAN",
            "_CRT_SECURE_NO_WARNINGS",
            "SENTINEL_DLL"
        }

        postbuildcommands
        {
            ("{COPY} ../bin/" .. outputdir .. "/Sentinel/Sentinel.dll ../bin/" .. outputdir .. "/Sandbox/")
        }

        ignoredefaultlibraries
        {
            "LIBCMTD",
            "LIBCMT"
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
