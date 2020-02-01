cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HelloTriangle)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    VertShader.hlsl
    PixelShader.hlsl
)

target_link_libraries(${PROJECT_NAME} PRIVATE Clove)
target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
set_source_files_properties(VertShader.hlsl PixelShader.hlsl PROPERTIES HEADER_FILE_ONLY TRUE)