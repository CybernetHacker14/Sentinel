# Engine CMake file

cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(INCLUDE_DIRS
	Source
	Vendor/GLFW/module/include
	Vendor/Glad/module/include
	Vendor/EASTL/module/include
	Vendor/spdlog/module/include
	Vendor/glm/module
)

set(LIB_LINKS
	glfw
	glad
	opengl32
	EASTL
)

if(WIN32)
	set(DX11_LIB_LINKS d3d11 dxgi dxguid d3dCompiler)
else()
	set(DX11_LIB_LINKS)
endif()

file(GLOB_RECURSE SOURCE_FILES
	Source/*.cpp
	Source/*.h
)

add_library(Sentinel STATIC ${SOURCE_FILES})
target_include_directories(Sentinel PUBLIC ${INCLUDE_DIRS})
target_link_libraries(Sentinel PUBLIC ${LIB_LINKS} ${DX11_LIB_LINKS})
target_precompile_headers(Sentinel PUBLIC "Source/stpch.h")

target_compile_definitions(
	Sentinel
	
	PRIVATE
	"_CRT_SECURE_NO_WARNINGS"
	"GLFW_INCLUDE_NONE"
	"USE_EASTL"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions("ST_DEBUG")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions("ST_RELEASE")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Dist")
	add_compile_definitions("ST_DIST")
endif()

set_property(TARGET Sentinel PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")