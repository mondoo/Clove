cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RotatingCube)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ExampleLayer.hpp
    ExampleLayer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Tunic)
target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

#Copy resource files from Garlic
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Garlic/Tunic/res
                ${CMAKE_CURRENT_BINARY_DIR}/res
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Garlic/Tunic/res
                ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/res
)