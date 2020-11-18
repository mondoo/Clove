set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Root)

#Library
add_library(
    Root.Expected INTERFACE
)

#Include
target_include_directories(
	Root.Expected 
	
	INTERFACE 
		include
)

if(GAR_BUILD_TESTS)
    add_subdirectory(tests)
endif()