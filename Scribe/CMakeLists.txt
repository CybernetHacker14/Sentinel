# Editor CMake file

cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(INCLUDE_DIRS Source Engine/Vendor)
file(GLOB_RECURSE EDITOR_SOURCE_FILES Source/*.cpp Source/*.h)

add_executable(SentinelEditor ${EDITOR_SOURCE_FILES})
target_link_libraries(SentinelEditor PRIVATE Sentinel)
target_include_directories(SentinelEditor PRIVATE ${INCLUDE_DIRS})

set_property(TARGET SentinelEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")