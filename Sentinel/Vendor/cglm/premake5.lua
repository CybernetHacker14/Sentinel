project "cglm"
    kind "StaticLib"
    language "C"
    cdialect "C11"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "module/include/**.h",
        "module/src/euler.c",
		"module/src/affine.c",
		"module/src/io.c",
		"module/src/quat.c",
		"module/src/cam.c",
		"module/src/vec2.c",
		"module/src/ivec2.c",
		"module/src/vec3.c",
		"module/src/ivec3.c",
		"module/src/vec4.c",
		"module/src/ivec4.c",
		"module/src/mat2.c",
		"module/src/mat2x3.c",
		"module/src/mat2x4.c",
		"module/src/mat3.c",
		"module/src/mat3x2.c",
		"module/src/mat3x4.c",
		"module/src/mat4.c",
		"module/src/mat4x2.c",
		"module/src/mat4x3.c",
		"module/src/plane.c",
		"module/src/frustum.c",
		"module/src/box.c",
		"module/src/project.c",
		"module/src/sphere.c",
		"module/src/ease.c",
		"module/src/curve.c",
		"module/src/bezier.c",
		"module/src/ray.c",
		"module/src/affine2d.c",
		"module/src/clipspace/ortho_lh_no.c",
		"module/src/clipspace/ortho_lh_zo.c",
		"module/src/clipspace/ortho_rh_no.c",
		"module/src/clipspace/ortho_rh_zo.c",
		"module/src/clipspace/persp_lh_no.c",
		"module/src/clipspace/persp_lh_zo.c",
		"module/src/clipspace/persp_rh_no.c",
		"module/src/clipspace/persp_rh_zo.c",
		"module/src/clipspace/view_lh_no.c",
		"module/src/clipspace/view_lh_zo.c",
		"module/src/clipspace/view_rh_no.c",
		"module/src/clipspace/view_rh_zo.c",
		"module/src/clipspace/project_no.c",
		"module/src/clipspace/project_zo.c"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        defines
        {
            "CGLM_STATIC"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"