project "zlib"
    kind "StaticLib"
    language "C"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "module/zlib.h",
        "module/crc32.h",
        "module/deflate.h",
        "module/gzguts.h",
        "module/inffast.h",
        "module/inffixed.h",
        "module/inflate.h",
        "module/inftrees.h",
        "module/trees.h",
        "module/zutil.h",
        "module/adler32.c",
		"module/compress.c",
		"module/crc32.c",
		"module/deflate.c",
		"module/gzclose.c",
		"module/gzlib.c",
		"module/gzread.c",
		"module/gzwrite.c",
		"module/inflate.c",
		"module/infback.c",
		"module/inftrees.c",
		"module/inffast.c",
		"module/trees.c",
		"module/uncompr.c",
		"module/zutil.c",
        "module/contrib/minizip/**.h",
        "module/contrib/minizip/**.c"
    }

    includedirs
    {
        "module"
    }

    filter "system:windows"
        staticruntime "On"
        cdialect "C11"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"