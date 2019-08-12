#Root CMake list
cmake_minimum_required(VERSION 3.14.0)
project(Clove)

##TODO: Add all of the platforms and configurations

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Defs - TEMP
add_compile_definitions(CLV_DEVELOPMENT=1 CLV_PLATFORM_WINDOWS=1)
#add_compile_options()

#Clove dependencies
add_subdirectory(Clove/vendor/dxerr) #TODO: Only add on windows?
add_subdirectory(Clove/vendor/Glad)
add_subdirectory(Clove/vendor/glm)
add_subdirectory(Clove/vendor/spdlog)
#TODO: add other vendor folders manually

#Clove
add_subdirectory(Clove)

#Sandbox
add_subdirectory(Sandbox)