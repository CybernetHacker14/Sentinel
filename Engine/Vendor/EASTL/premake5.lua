project "EASTL"
    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "module/include/**.h",
        "module/source/**.cpp",
        "module/test/packages/EABase/include/Common/**.h",
        "module/test/packages/EABase/include/Common/**.cpp"
    }

    includedirs
    {
        "module/include",
        "module/test/packages/EABase/include/Common"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"