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

set(PA_BUILD_SHARED OFF CACHE BOOL "Build shared/dynamic library")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(PA_USE_DS ON CACHE BOOL "Enable support for DirectSound")
	set(PA_USE_ASIO OFF CACHE BOOL "Enable support for ASIO")
	set(PA_USE_WMME OFF CACHE BOOL "Enable support for MME")
	set(PA_USE_WASAPI OFF CACHE BOOL "Enable support for WASAPI")
	set(PA_USE_WDMKS OFF CACHE BOOL "Enable support for WDMKS")
	set(PA_USE_WDMKS_DEVICE_INFO OFF CACHE BOOL "Use WDM/KS API for device info")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(PA_USE_JACK OFF CACHE BOOL "Enable support for Jack")
	set(PA_USE_ALSA ON CACHE BOOL "Enable support for ALSA")
endif()
add_subdirectory(Clove/vendor/portaudio)

set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
#set(BUILD_TESTING OFF CACHE BOOL)
set(ENABLE_CPACK ON CACHE BOOL "Enable CPack support")
if (MSVC OR MINGW)
	set (ENABLE_STATIC_RUNTIME ON CACHE BOOL "Enable static runtime")
endif()
add_subdirectory(Clove/vendor/libsndfile)

#Clove
add_subdirectory(Clove)

#Sandbox
add_subdirectory(Sandbox)
