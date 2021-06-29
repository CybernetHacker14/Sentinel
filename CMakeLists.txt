# Project CMake file

cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Sentinel-Engine
		VERSION 0.0
		DESCRIPTION "A C++17 based Game Engine"
		LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

# Prevents in-source builds
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Please make a new directory and (called a build directory) and run CMake from there.")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /std:c++17 /W4 /arch:AVX2 /fp:fast /MP /DNOMINMAX")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
		
# Vendor
add_subdirectory(Engine/Vendor/GLFW/module)
add_subdirectory(Engine/Vendor/EASTL/module)
add_subdirectory(Engine/Vendor/Glad/module)

# Sentinel-Engine
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Sandbox)

# CMake

# Group projects together in categories
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(EASTL PROPERTIES FOLDER "Dependencies")

set_target_properties(Sentinel PROPERTIES FOLDER "Core Modules")
set_target_properties(SentinelEditor PROPERTIES FOLDER "Core Modules")

set_target_properties(Sandbox PROPERTIES FOLDER "Test Modules")

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "Sandbox" )