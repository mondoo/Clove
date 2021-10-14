option(CLOVE_ENABLE_PROFILING "Enables the Clove profiler which will print function timings to a file" OFF)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    CloveTimer STATIC
        ${INCLUDE}/Timer.hpp
	    ${SOURCE}/Timer.cpp
)

#Include
target_include_directories(
	CloveTimer
	
	PUBLIC
		include
)

#Definitions
target_compile_definitions(
    CloveTimer

    PUBLIC
		CLOVE_ENABLE_PROFILING=$<BOOL:${CLOVE_ENABLE_PROFILING}>
)