include "Premake/customization/solution_items.lua"
include "Premake/customization/clean_project.lua"
include "Premake/customization/cmdargs.lua"
include "Premake/customization/vendor_includes.lua"

workspace "Sentinel"
    architecture "x86_64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
		"Release",
		"Dist"
    }

    solution_items
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include dependencies
group "Dependencies"
    include "Premake"
    include "Sentinel/Vendor/glfw"
    include "Sentinel/Vendor/cglm"
group ""

-- Include projects
group "Core Modules"
    include "Sentinel"
group ""

group "Test Modules"
    include "Sandbox"
group ""
