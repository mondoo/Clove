#Clove CMake list

project(Clove)

file(
	GLOB clove_src
    src/*.hpp
	src/*.cpp
	src/Clove/*.hpp
    src/Clove/*.cpp
)

add_library(
	Clove STATIC
	${clove_src}
)

target_link_libraries(
	Clove PRIVATE 
	glm
	spdlog
)