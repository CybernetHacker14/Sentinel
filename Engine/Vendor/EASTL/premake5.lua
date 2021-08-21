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
        "module/test/packages/EABase/include/Common/**.cpp",
		"module/test/packages/EAAssert/include/**.h",
		"module/test/packages/EAAssert/source/**.cpp",
		"module/test/packages/EAStdC/include/**.h",
		"module/test/packages/EAStdC/source/**.cpp"
    }

    includedirs
    {
        "module/include",
        "module/test/packages/EABase/include/Common",
		"module/test/packages/EAStdC/include",
		"module/test/packages/EAAssert/include",
		"module/test/packages/EAThread/include"
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