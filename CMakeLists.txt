#Root CMake list
cmake_minimum_required(VERSION 3.15.0)
project(Clove-Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSharp_FLAGS "/langversion:8.0")

#Build configurations
set(debug_flags 		"-DCLV_DEBUG=1 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=0")
set(development_flags 	"-DCLV_DEBUG=0 -DCLV_DEVELOPMENT=1 -DCLV_PROFILING=0 -DCLV_RELEASE=0")
set(profiling_flags 	"-DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=1 -DCLV_RELEASE=0")
set(release_flags 		"-DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=1")

set(cxx_debug_flags)
set(cxx_development_flags)
set(cxx_profiling_flags)
set(cxx_release_flags)

if(MSVC)
	set(cxx_debug_flags 		"/Od /Ob0 /Zi /RTC1 /Wall ${debug_flags}")
	set(cxx_development_flags 	"/Od /Ob0 /Zi /RTC1 /W3 ${development_flags}")
	set(cxx_profiling_flags 	"/DNDEBUG /O2 /Ob3 /W0 ${profiling_flags}")
	set(cxx_release_flags 		"/DNDEBUG /O2 /Ob3 /W0 ${release_flags}")

	set(CMAKE_EXE_LINKER_FLAGS_DEBUG		"/debug /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT	"/debug /INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_PROFILING	"/INCREMENTAL:NO")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE		"/INCREMENTAL:NO")

	#this does a weird thing with C++/CLR
	#set(CMAKE_SHARED_LINKER_FLAGS_DEBUG			"/debug /INCREMENTAL")
	#set(CMAKE_SHARED_LINKER_FLAGS_DEVELOPMENT	"/debug /INCREMENTAL")
	#set(CMAKE_SHARED_LINKER_FLAGS_PROFILING		"/INCREMENTAL:NO")
	#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE		"/INCREMENTAL:NO")
else()
	set(cxx_debug_flags 		"-O0 -g -Wall -Wextra ${debug_flags}")
	set(cxx_development_flags 	"-O0 -g -Wall ${development_flags}")
	set(cxx_profiling_flags 	"-DNDEBUG -O3 -w ${profiling_flags}")
	set(cxx_release_flags 		"-DNDEBUG -O3 -w ${release_flags}")
endif()

set(csharp_debug_flags			"/debug:full /optimize- /warn:3 /errorreport:prompt /define:DEBUG")
set(csharp_development_flags	"/debug:full /optimize- /warn:3 /errorreport:prompt /define:DEBUG")
set(csharp_profiling_flags		"/debug:none /optimize")
set(csharp_release_flags		"/debug:none /optimize")

#Set custom build types
set(CMAKE_CONFIGURATION_TYPES Debug Development Profiling Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Custom Clove configuration types" FORCE)

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

set(GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests")
set(PA_BUILD_SHARED OFF CACHE BOOL "Build shared/dynamic library")
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")

#Clove dependencies
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(EngineFolder "Clove")
set(EditorFolder "${EngineFolder}/Editor")
set(DependencyFolder "${EngineFolder}/Dependencies")

#-dxerr
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(Clove/vendor/dxerr)
	set_target_properties(dxerr PROPERTIES FOLDER ${DependencyFolder})
endif()

#-Glad
add_subdirectory(Clove/vendor/Glad)
set_target_properties(Glad PROPERTIES FOLDER ${DependencyFolder})

#-glm
add_subdirectory(Clove/vendor/glm)
set_target_properties(glm_static PROPERTIES FOLDER ${DependencyFolder}/glm)
set_target_properties(uninstall PROPERTIES FOLDER ${DependencyFolder}/glm)

#-spdlog
add_subdirectory(Clove/vendor/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER ${DependencyFolder})

#-stb
add_subdirectory(Clove/vendor/stb)
set_target_properties(stb PROPERTIES FOLDER ${DependencyFolder})

#-portaudio
add_subdirectory(Clove/vendor/portaudio)
set_target_properties(portaudio_static PROPERTIES FOLDER ${DependencyFolder})

#-libsndfile
add_subdirectory(Clove/vendor/libsndfile)
set_target_properties(sndfile PROPERTIES FOLDER ${DependencyFolder})

#Clove
add_subdirectory(Clove)
set_target_properties(Clove PROPERTIES FOLDER ${EngineFolder})

#Sandbox
add_subdirectory(Sandbox)
set_target_properties(Sandbox PROPERTIES FOLDER ${EngineFolder})

#Editor
if(MSVC) #not currently supported without MSVC
	add_subdirectory(Editor)
	set_target_properties(CloveWrapper PROPERTIES FOLDER ${EditorFolder})
	set_target_properties(CloveEditor PROPERTIES FOLDER ${EditorFolder})
endif()