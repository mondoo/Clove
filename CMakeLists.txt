#Root CMake list
cmake_minimum_required(VERSION 3.16.0)
project(Garlic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSharp_FLAGS "/langversion:8.0")

#Build configurations
if(MSVC)
	set(cxx_debug_flags 		"/Od /Ob0 /Zi /RTC1 /Wall")
	set(cxx_development_flags 	"/Od /Ob0 /Zi /RTC1 /W3")
	set(cxx_profiling_flags 	"/DNDEBUG /O2 /Ob3 /W0")
	set(cxx_release_flags 		"/DNDEBUG /O2 /Ob3 /W0")

	set(CMAKE_EXE_LINKER_FLAGS_DEBUG		"/debug /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT	"/debug /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_PROFILING	"/INCREMENTAL:NO")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE		"/INCREMENTAL:NO")

	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG			"/debug /INCREMENTAL")
	set(CMAKE_SHARED_LINKER_FLAGS_DEVELOPMENT	"/debug /INCREMENTAL")
	set(CMAKE_SHARED_LINKER_FLAGS_PROFILING		"/INCREMENTAL:NO")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE		"/INCREMENTAL:NO")
else()
	set(cxx_debug_flags 		"-O0 -g -Wall -Wextra")
	set(cxx_development_flags 	"-O0 -g -Wall")
	set(cxx_profiling_flags 	"-DNDEBUG -O3 -w")
	set(cxx_release_flags 		"-DNDEBUG -O3 -w")
endif()

set(csharp_debug_flags			"/debug:full /optimize- /warn:4 /errorreport:prompt /define:DEBUG")
set(csharp_development_flags	"/debug:full /optimize- /warn:3 /errorreport:prompt /define:DEBUG")
set(csharp_profiling_flags		"/debug:none /optimize")
set(csharp_release_flags		"/debug:none /optimize")

#Set custom build types
set(CMAKE_CONFIGURATION_TYPES Debug Development Profiling Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Custom Garlic configuration types" FORCE)

set(CMAKE_C_FLAGS_DEBUG				"${cxx_debug_flags}")
set(CMAKE_C_FLAGS_DEVELOPMENT		"${cxx_development_flags}")
set(CMAKE_C_FLAGS_PROFILING			"${cxx_profiling_flags}")
set(CMAKE_C_FLAGS_RELEASE			"${cxx_release_flags}")

set(CMAKE_CXX_FLAGS_DEBUG			"${cxx_debug_flags}")
set(CMAKE_CXX_FLAGS_DEVELOPMENT		"${cxx_development_flags}")
set(CMAKE_CXX_FLAGS_PROFILING		"${cxx_profiling_flags}")
set(CMAKE_CXX_FLAGS_RELEASE			"${cxx_release_flags}")

set(CMAKE_CSharp_FLAGS				"${CMAKE_CSharp_FLAGS} /platform:x64") #Note forcing it to x64 but this should be what ever the wrapper dll gets built as (anycpu seems to prefer 32 bit)
set(CMAKE_CSharp_FLAGS_DEBUG		"${csharp_debug_flags}")
set(CMAKE_CSharp_FLAGS_DEVELOPMENT	"${csharp_development_flags}")
set(CMAKE_CSharp_FLAGS_PROFILING	"${csharp_profiling_flags}")
set(CMAKE_CSharp_FLAGS_RELEASE		"${csharp_release_flags}")

#default settings
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree")

#-custom
option(GAR_BUILD_BULB "Build the Bulb editor" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Clove
add_subdirectory(Clove)

#Builb
if(MSVC AND GAR_BUILD_BULB) #not currently supported without MSVC
	add_subdirectory(Bulb)
endif()