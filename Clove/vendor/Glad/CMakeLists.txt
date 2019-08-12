project(Glad)

add_library(
    Glad
    STATIC
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)