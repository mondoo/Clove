#Root CMake list
cmake_minimum_required(VERSION 3.16.0)
project(Garlic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Build configurations
if(MSVC)
	set(debug_flags 		"/Od /Ob0 /Zi /RTC1 /Wall")
	set(development_flags 	"/Od /Ob0 /Zi /RTC1 /W3")
	set(profiling_flags 	"/DNDEBUG /O2 /Ob3 /W0")
	set(release_flags 		"/DNDEBUG /O2 /Ob3 /W0")

	set(CMAKE_EXE_LINKER_FLAGS_DEBUG		"/debug /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT	"/debug /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_PROFILING	"/INCREMENTAL:NO")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE		"/INCREMENTAL:NO")
else()
	set(debug_flags 		"-O0 -g -Wall -Wextra")
	set(development_flags 	"-O0 -g -Wall")
	set(profiling_flags 	"-DNDEBUG -O3 -w")
	set(release_flags 		"-DNDEBUG -O3 -w")
endif()

#Set custom build types
set(CMAKE_CONFIGURATION_TYPES Debug Development Profiling Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Custom Clove configuration types" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "${debug_flags}")
set(CMAKE_C_FLAGS_DEBUG "${debug_flags}")

set(CMAKE_CXX_FLAGS_DEVELOPMENT "${development_flags}")
set(CMAKE_C_FLAGS_DEVELOPMENT "${development_flags}")

set(CMAKE_CXX_FLAGS_PROFILING "${profiling_flags}")
set(CMAKE_C_FLAGS_PROFILING "${profiling_flags}")

set(CMAKE_CXX_FLAGS_RELEASE "${release_flags}")
set(CMAKE_C_FLAGS_RELEASE "${release_flags}")

#Garlic settings
#-defaults
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree")

#-custom
option(GAR_BUILD_SANDBOX "Build the Sandbox project used for testing" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Clove
add_subdirectory(Clove)

#Sandbox
if(GAR_BUILD_SANDBOX)
	add_subdirectory(Sandbox)
endif()