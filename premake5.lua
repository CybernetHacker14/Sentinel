include "Premake/customization/solution_items.lua"
include "Premake/customization/clean_project.lua"

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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Sentinel_Source"] = "%{wks.location}/Engine/Source"
IncludeDir["Sentinel_Vendor"] = "%{wks.location}/Engine/Vendor"
IncludeDir["GLFW"] = "%{wks.location}/Engine/Vendor/GLFW/module/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/Vendor/Glad/module/include"
IncludeDir["glm"] = "%{wks.location}/Engine/Vendor/glm/module"
IncludeDir["spdlog"] = "%{wks.location}/Engine/Vendor/spdlog/module/include"
IncludeDir["EASTL"] = "%{wks.location}/Engine/Vendor/EASTL/module/include"
IncludeDir["EABase"] = "%{wks.location}/Engine/Vendor/EASTL/module/test/packages/EABase/include/Common"

-- Include dependencies
group "Dependencies"
    include "Premake"
    include "Engine/Vendor/GLFW"
    include "Engine/Vendor/Glad"
    include "Engine/Vendor/EASTL"
group ""

-- Include projects
group "Core Modules"
    include "Engine"
    include "Editor"
group ""

group "Test Modules"
    include "Sandbox"
group ""
