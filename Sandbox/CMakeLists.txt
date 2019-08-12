#Sandbox CMake list

project(Sandbox)

file(
	GLOB sandbox_src
    src/*.hpp
    src/*.cpp
)

add_executable(
	Sandbox
	${sandbox_src}
)

target_include_directories(
	Sandbox
	PUBLIC
		src
)

target_link_libraries(Sandbox PRIVATE Engine)
