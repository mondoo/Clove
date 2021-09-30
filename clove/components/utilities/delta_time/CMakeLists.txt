set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)

#Library
add_library(
    CloveDeltaTime STATIC
        ${INCLUDE}/DeltaTime.hpp
	    ${SOURCE}/DeltaTime.cpp
)

#Include
target_include_directories(
	CloveDeltaTime
	
	PUBLIC
		include
)

copy_target_includes_to_out_dir(CloveDeltaTime)