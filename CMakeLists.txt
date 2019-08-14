#Root CMake list
cmake_minimum_required(VERSION 3.14.0)
project(Clove-Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Clove dependencies
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory(Clove/vendor/dxerr)
endif()

add_subdirectory(Clove/vendor/Glad)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(GLM_TEST_ENABLE  OFF CACHE BOOL "Build unit tests" FORCE)
add_subdirectory(Clove/vendor/glm)

set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(Clove/vendor/spdlog)

add_subdirectory(Clove/vendor/stb)

#Clove
add_subdirectory(Clove)

#Sandbox
add_subdirectory(Sandbox)
