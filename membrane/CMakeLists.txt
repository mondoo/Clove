set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Membrane)

#Library
add_library(
    Membrane SHARED
        ${INCLUDE}/Application.hpp
        ${SOURCE}/Application.cpp

        ${INCLUDE}/Log.hpp
        ${SOURCE}/Log.cpp

        ${INCLUDE}/EditorTypes.hpp

        ${INCLUDE}/MessageHandler.hpp
        ${SOURCE}/MessageHandler.cpp
        ${INCLUDE}/Messages.hpp

        ${INCLUDE}/EditorLayer.hpp
        ${SOURCE}/EditorLayer.cpp
        ${INCLUDE}/RuntimeLayer.hpp
        ${SOURCE}/RuntimeLayer.cpp
)

#Include
target_include_directories(
    Membrane

    PUBLIC
        include   
)

#Libraries
target_link_libraries(
    Membrane

    PRIVATE
        Clove
)

#Definitions
target_compile_definitions(
    Membrane

    PRIVATE
        ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

#Properties
set_target_properties(
    Membrane
    
    PROPERTIES 
        COMMON_LANGUAGE_RUNTIME ""
        VS_DOTNET_REFERENCES "System;PresentationCore;PresentationFramework;WindowsBase"
        CXX_STANDARD 17
)