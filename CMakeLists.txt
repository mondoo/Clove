cmake_minimum_required(VERSION 3.16.0)

include(CMakeDependentOption)

project(Garlic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSharp_FLAGS "/langversion:8.0")

#default settings
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library")
set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree")

#custom settings
option(GAR_BUILD_TUNIC "Build the Tunic framework" ON)
CMAKE_DEPENDENT_OPTION(GAR_BUILD_BULB "Build the Bulb editor (requries Tunic)" OFF "GAR_BUILD_TUNIC" OFF)
option(GAR_BUILD_EXAMPLES OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Clove
add_subdirectory(Clove)

#Tunic
if(GAR_BUILD_TUNIC)
	add_subdirectory(Tunic)
endif()

#Builb
if(MSVC AND GAR_BUILD_BULB) #not currently supported without MSVC
	add_subdirectory(Bulb)
endif()