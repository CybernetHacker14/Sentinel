project "minizip"
	kind "StaticLib"
	language "C"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"module/mz.h",
        "module/mz_compat.c",
        "module/mz_compat.h",
        "module/mz_crypt.c",
        "module/mz_crypt.h",
        "module/mz_os.c",
        "module/mz_os.h",
        "module/mz_os_win32.c",
        "module/mz_strm.c",
        "module/mz_strm.h",
        "module/mz_strm_buf.c",
        "module/mz_strm_buf.h",
        "module/mz_strm_mem.c",
        "module/mz_strm_mem.h",
        "module/mz_strm_os.h",
        "module/mz_strm_os_win32.c",
        "module/mz_strm_pkcrypt.c",
        "module/mz_strm_pkcrypt.h",
        "module/mz_strm_split.c",
        "module/mz_strm_split.h",
        "module/mz_strm_wzaes.c",
        "module/mz_strm_wzaes.h",
        "module/mz_zip.c",
        "module/mz_zip.h",
        "module/mz_zip_rw.c",
        "module/mz_zip_rw.h",
        "module/minizip.c"
	}

    includedirs
    {
        "module"
    }

    defines
    {
        "HAVE_PKCRYPT"
    }
	
	filter "system:windows"
        systemversion "latest"
        cdialect "C11"
        staticruntime "On"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        cdialect "C11"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"