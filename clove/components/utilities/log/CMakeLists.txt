option(CLOVE_ENABLE_ASSERTIONS "Enables the CLOVE_ASSERT macro. Allowing the program to debug break if certain conditions fail." ON)

set(CLOVE_LOG_LEVEL 0 CACHE STRING "Sets the level for logs allowed to output in the console. 0 (default) is info and up, 1 is debug and up and 2 is all.")
set_property(CACHE CLOVE_LOG_LEVEL PROPERTY STRINGS 0 1 2)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Log)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    CloveLog STATIC
        ${INCLUDE}/Log.hpp
	    ${INCLUDE}/Log.inl
	    ${SOURCE}/Log.cpp
)

#Include
target_include_directories(
	CloveLog
	
	PUBLIC
		include
)

#Libraries
target_link_libraries(
    CloveLog

    PUBLIC
        CONAN_PKG::spdlog

    PRIVATE
        CloveDefinitions
)

#Preprocessor
target_compile_definitions(
	CloveLog

	PUBLIC
		CLOVE_ENABLE_ASSERTIONS=$<BOOL:${CLOVE_ENABLE_ASSERTIONS}>

	PRIVATE
		CLOVE_LOG_LEVEL=${CLOVE_LOG_LEVEL}
)