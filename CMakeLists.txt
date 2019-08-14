#Root CMake list
cmake_minimum_required(VERSION 3.14.0)
project(Clove-Engine)

##TODO: Add all of the platforms and configurations

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Defs - TEMP
#add_compile_definitions(CLV_DEVELOPMENT=1 CLV_PLATFORM_WINDOWS=1)
#add_compile_options()

#Clove dependencies
add_subdirectory(Clove/vendor/dxerr) #TODO: Only add on windows?

add_subdirectory(Clove/vendor/Glad)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(GLM_TEST_ENABLE  OFF CACHE BOOL "Build unit tests" FORCE)
add_subdirectory(Clove/vendor/glm)

set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(Clove/vendor/spdlog)

add_subdirectory(Clove/vendor/stb)
#TODO: add other vendor folders manually

#Clove
add_subdirectory(Clove)

#Sandbox
add_subdirectory(Sandbox)

#set_target_properties(dxerr PROPERTIES FOLDER Dependencies)
#set_target_properties(Glad PROPERTIES FOLDER Dependencies)
#set_target_properties(glm PROPERTIES FOLDER Dependencies)
#set_target_properties(spdlog PROPERTIES FOLDER Dependencies)
#set_target_properties(stb PROPERTIES FOLDER Dependencies)