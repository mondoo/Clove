project(Vulkan)
set(ApplicationFolder "Garlic/Vulkan")

add_executable(
	${PROJECT_NAME}
	source/main.cpp
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
		Vulkan::Vulkan
)

#Definitions
target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
)