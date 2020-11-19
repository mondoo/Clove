set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Scape/Wrapper)

add_library(
    Scape.Wrapper STATIC
        ${INCLUDE}/Application.hpp
        ${SOURCE}/Application.cpp
)

target_include_directories(
    Scape.Wrapper

    PUBLIC
        include   
)

target_link_libraries(
    Scape.Wrapper

    PRIVATE
        Stem
)

target_compile_definitions(
    Scape.Wrapper
    PRIVATE
        ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)