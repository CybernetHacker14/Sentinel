project "Sandbox"
    kind "ConsoleApp"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.c",
    }

    includedirs
    {
        "%{wks.location}/Sentinel/Source/Runtime/_EXPORT",
        "%{VendorIncludes.cglm}"
    }

    links
    {
        "Sentinel"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "WIN32_LEAN_AND_MEAN",
            "_CRT_SECURE_NO_WARNINGS"
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