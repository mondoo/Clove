#Root CMake list
cmake_minimum_required(VERSION 3.14.0)
project(Clove)
set(CMAKE_BUILD_TYPE Debug)

#Clove dependencies
add_subdirectory(Clove/vendor/glm)
add_subdirectory(Clove/vendor/spdlog)
#TODO: add other vendor folders manually

#Clove
add_subdirectory(Clove)

#Sandbox
add_subdirectory(Sandbox)

#CMAKE_BUILD_TYPE(Debug)