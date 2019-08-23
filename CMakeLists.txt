#Root CMake list
cmake_minimum_required(VERSION 3.15.0)
project(Clove-Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(EngineFolder "Clove")
set(DependencyFolder "Clove/Dependencies")

#Build configurations
if(CMAKE_CONFIGURATION_TYPES)
	#Set custom build types
	set(CMAKE_CONFIGURATION_TYPES Debug Development Profiling Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)

	#Set the custom build flags to the defaults
	set(CMAKE_CXX_FLAGS_DEVELOPMENT "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_C_FLAGS_DEVELOPMENT "${CMAKE_C_FLAGS_DEBUG}")

	set(CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_C_FLAGS_PROFILING "${CMAKE_C_FLAGS_RELEASE}")

	#Set the defines for build configuration
	#TODO: Anyway to put these as an PUBLIC flag inside clove?
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCLV_DEBUG=1 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=0")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCLV_DEBUG=1 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=0")

	set(CMAKE_CXX_FLAGS_DEVELOPMENT "${CMAKE_CXX_FLAGS_DEVELOPMENT} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=1 -DCLV_PROFILING=0 -DCLV_RELEASE=0")
	set(CMAKE_C_FLAGS_DEVELOPMENT "${CMAKE_C_FLAGS_DEVELOPMENT} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=1 -DCLV_PROFILING=0 -DCLV_RELEASE=0")

	set(CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_PROFILING} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=1 -DCLV_RELEASE=0")
	set(CMAKE_C_FLAGS_PROFILING "${CMAKE_C_FLAGS_PROFILING} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=1 -DCLV_RELEASE=0")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=1")
else()
	#Set up defines for non configuration generators
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCLV_DEBUG=1 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=0")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCLV_DEBUG=1 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=0")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCLV_DEBUG=0 -DCLV_DEVELOPMENT=0 -DCLV_PROFILING=0 -DCLV_RELEASE=1")
endif()

#Clove dependencies
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(Clove/vendor/dxerr)
	set_target_properties(dxerr PROPERTIES FOLDER ${DependencyFolder})
endif()

#-Glad
add_subdirectory(Clove/vendor/Glad)
set_target_properties(Glad PROPERTIES FOLDER ${DependencyFolder})

#-glm
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(GLM_TEST_ENABLE  OFF CACHE BOOL "Build unit tests" FORCE)
add_subdirectory(Clove/vendor/glm)
set_target_properties(glm_static PROPERTIES FOLDER ${DependencyFolder})

#-spdlog
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(Clove/vendor/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER ${DependencyFolder})

#-stb
add_subdirectory(Clove/vendor/stb)
set_target_properties(stb PROPERTIES FOLDER ${DependencyFolder})

#Clove
add_subdirectory(Clove)
set_target_properties(Clove PROPERTIES FOLDER ${EngineFolder})

#Sandbox
add_subdirectory(Sandbox)
set_target_properties(Sandbox PROPERTIES FOLDER ${EngineFolder})