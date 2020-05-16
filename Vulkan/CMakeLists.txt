project(Vulkan)
set(ApplicationFolder "Garlic/Vulkan")

#compile spirv
add_custom_command(
	OUTPUT vert.spirv
	COMMAND glslc -fshader-stage=vertex ${CMAKE_CURRENT_SOURCE_DIR}/source/vert.glsl -o vert.spirv
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/vert.glsl
)

add_custom_command(
	OUTPUT frag.spirv
	COMMAND glslc -fshader-stage=fragment ${CMAKE_CURRENT_SOURCE_DIR}/source/frag.glsl -o frag.spirv
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/frag.glsl
)

add_executable(
	${PROJECT_NAME}
	source/main.cpp
	source/vert.glsl
	source/frag.glsl

	vert.spirv
	frag.spirv
)
set_target_properties(Vulkan PROPERTIES FOLDER ${ApplicationFolder})

add_subdirectory(vendor/glfw)
set_target_properties(glfw PROPERTIES FOLDER ${ApplicationFolder})

#For Vulkan
find_package(Vulkan REQUIRED)

#Includes
target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/source
	PRIVATE
		Vulkan::Vulkan
)

#Libraries
target_link_libraries(
	${PROJECT_NAME}
	PRIVATE 
		glfw
		glm_static
		stb
		Vulkan::Vulkan
)

#Definitions
target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
		RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resource"
)