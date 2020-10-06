option(GAR_ENABLE_MEMORY_DEBUGGING "Enables certain memory debugging tools such as extra logs" OFF)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Root/Memory)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    Root.Memory STATIC
        ${INCLUDE}/ListAllocator.hpp
		${SOURCE}/ListAllocator.cpp
		${INCLUDE}/PoolAllocator.hpp
		${INCLUDE}/PoolAllocator.inl
		${INCLUDE}/StackAllocator.hpp
		${SOURCE}/StackAllocator.cpp
)

#Include
target_include_directories(
	Root.Memory 
	
	PUBLIC
		include
)

#Libraries
target_link_libraries(
    Root.Memory 

    PRIVATE
        Root.Definitions
)

#Preprocessor
target_compile_definitions(
	Root.Memory

	#General
	PRIVATE
		GAR_ENABLE_MEMORY_DEBUGGING=$<BOOL:${GAR_CLV_ENABLE_MEMORY_DEBUGGING}>
)