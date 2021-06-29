# Glad CMake file

cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Glad
		LANGUAGES C)
		
set(INCLUDE_DIR
	include/
)

file(GLOB_RECURSE SOURCE
	src/*.c
)

add_library(glad STATIC ${INCLUDE_DIR} ${SOURCE})
target_include_directories(glad PUBLIC ${INCLUDE_DIR})
