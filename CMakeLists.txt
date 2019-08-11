cmake_minimum_required(VERSION 3.15.0)

project(Clove)


#Clove dependencies
add_subdirectory(Clove/vendor/glm)
add_subdirectory(Clove/vendor/spdlog)
#TODO: add other vendor folders manually

#Clove
add_subdirectory(Clove)