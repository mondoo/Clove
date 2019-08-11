add_library(GLOB Clove STATIC
    *.hpp
    *.cpp
)

target_link_libraries(Clove PRIVATE glm)
