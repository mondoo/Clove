cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RotatingCube)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ExampleLayer.hpp
    ExampleLayer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Bulb)
target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")