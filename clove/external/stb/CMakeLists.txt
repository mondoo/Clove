project(stb)

add_library(
	stb 
	STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image.h
	stb_image.cpp
)

target_include_directories(
	stb
	PUBLIC
		include
)