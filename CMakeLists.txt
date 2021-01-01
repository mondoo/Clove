cmake_minimum_required(VERSION 3.18.0)

enable_testing()

project(Garlic LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Required for linking shared object (*.so) files on Linux
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#Make pthread is included while linked (still sometimes requires this in external projects when used as a sub_directory)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#default settings
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library")
set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree") 

#Garlic settings
option(GAR_BUILD_BULB "Build Bulb, Garlic's editor" ON)
option(GAR_BUILD_TESTS "Build Garlic's tests" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Dependencies
add_subdirectory(external)

#Engine
add_subdirectory(clove)

#Editor
if(GAR_BUILD_BULB AND MSVC)
	add_subdirectory(membrane)
	add_subdirectory(bulb)
endif()