cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SkeletalAnimation)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Bulb)
target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")