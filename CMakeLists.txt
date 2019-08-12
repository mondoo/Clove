#Root CMake list
cmake_minimum_required(VERSION 3.10.0)
project(Clove)

##TODO: Add all of the platforms and configurations

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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