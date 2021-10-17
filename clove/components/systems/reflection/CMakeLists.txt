set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Reflection)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    CloveReflection INTERFACE
)

#Includes
target_include_directories(
    CloveReflection
    
	INTERFACE
		include
)

if(CLOVE_BUILD_TESTS)
	add_subdirectory(tests)
endif()