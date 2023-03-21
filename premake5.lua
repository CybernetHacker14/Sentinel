include "Premake/customization/solution_items.lua"
include "Premake/customization/clean_project.lua"
include "Premake/customization/cmdargs.lua"
include "Premake/customization/shader_file_locations.lua"
include "Premake/customization/internal_dependencies.lua"
include "Premake/customization/external_dependencies.lua"

workspace "Sentinel"
    architecture "x86_64"
    startproject "Scribe"

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
    include "Engine/Vendor/GLFW"
    include "Engine/Vendor/EABase"
    include "Engine/Vendor/EASTL"
    include "Engine/Vendor/cereal"
    include "Engine/Vendor/imgui"
    include "Engine/Vendor/flecs"
    include "Engine/Vendor/minizip"
group ""

-- Include projects
group "Core Modules"
    include "Engine"
    include "Scribe"
group ""

group "Test Modules"
    include "Sandbox"
group ""
