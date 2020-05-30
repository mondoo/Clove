project(VKBackendTest)
set(ApplicationFolder "Garlic/VKBackendTest")

#compile spirv
#add_custom_command(
#	OUTPUT vert.spirv
#	COMMAND glslc -fshader-stage=vertex ${CMAKE_CURRENT_SOURCE_DIR}/source/vert.glsl -o vert.spirv
#	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/vert.glsl
#)
#
#add_custom_command(
#	OUTPUT frag.spirv
#	COMMAND glslc -fshader-stage=fragment ${CMAKE_CURRENT_SOURCE_DIR}/source/frag.glsl -o frag.spirv
#	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/frag.glsl
#)

add_executable(
	${PROJECT_NAME}
	main.cpp
	#source/vert.glsl
	#source/frag.glsl

	#vert.spirv
	#frag.spirv
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${ApplicationFolder})

#add_subdirectory(vendor/glfw)
#set_target_properties(glfw PROPERTIES FOLDER ${ApplicationFolder})

#For Vulkan
#find_package(Vulkan REQUIRED)

#Includes
#target_include_directories(
#	${PROJECT_NAME}
#	PUBLIC
#		${CMAKE_CURRENT_SOURCE_DIR}/source
#	PRIVATE
#		Vulkan::Vulkan
#)

#Libraries
target_link_libraries(
	${PROJECT_NAME}
	PRIVATE 
		Bulb

		#Vulkan::Vulkan
		#glfw
		#stb
)

#Definitions
#target_compile_definitions(
#	${PROJECT_NAME}
#	PRIVATE
#		RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resource"
#)
#target_compile_definitions(
#	glm_static
#	PUBLIC
#		GLM_FORCE_RADIANS
#		GLM_FORCE_LEFT_HANDED
#		GLM_FORCE_DEPTH_ZERO_TO_ONE
#)