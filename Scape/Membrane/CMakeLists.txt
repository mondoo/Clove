set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Scape/Membrane)

add_library(
    Scape.Membrane SHARED
        ${INCLUDE}/Application.hpp
        ${SOURCE}/Application.cpp
)

target_include_directories(
    Scape.Membrane

    PUBLIC
        include   
)

target_link_libraries(
    Scape.Membrane

    PUBLIC
        Scape.Wrapper
)

set_target_properties(Scape.Membrane 
    PROPERTIES 
        COMMON_LANGUAGE_RUNTIME ""
        VS_DOTNET_REFERENCES "System;PresentationCore;PresentationFramework;WindowsBase"
        CXX_STANDARD 17
)