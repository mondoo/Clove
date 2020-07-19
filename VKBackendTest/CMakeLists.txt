project(VKBackendTest)
set(ApplicationFolder "Garlic/VKBackendTest")

include(../Clove/cmake/CompileShader.cmake)

compileShader("${CMAKE_CURRENT_SOURCE_DIR}/vert.glsl" "vert.spirv" "vertex")
compileShader("${CMAKE_CURRENT_SOURCE_DIR}/frag.glsl" "frag.spirv" "fragment")

add_executable(
	${PROJECT_NAME}
	main.cpp
	vert.glsl
	frag.glsl

	vert.spirv
	frag.spirv
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${ApplicationFolder})
target_compile_definitions(${PROJECT_NAME} PRIVATE SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

#Libraries
target_link_libraries(
	${PROJECT_NAME}
	PRIVATE 
		Bulb
)