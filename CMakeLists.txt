cmake_minimum_required(VERSION 3.18.0)

enable_testing()

project(Garlic LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSharp_FLAGS "/langversion:8.0")

#Required for linking shared object (*.so) files on Linux
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#default settings
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library")
set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree") 

#Garlic settings
option(GAR_BUILD_SCAPE "Build Scape, Garlic's editor" ON)
option(GAR_BUILD_TESTS "Build Garlic's tests" OFF)

#Clove settings
option(GAR_CLV_IMGUI_COMPATIBLE "Links Glad publicly so users can initialise Dear ImGui properly" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Garlic's dependencies
add_subdirectory(external)

#Garlic's modules
add_subdirectory(clove)
add_subdirectory(Bulb)
add_subdirectory(Stem)

#Garlic's editor
if(GAR_BUILD_SCAPE AND MSVC)
	add_subdirectory(Scape)
endif()