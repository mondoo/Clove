project(Vulkan)
set(ApplicationFolder "Garlic/Vulkan")

add_executable(
	${PROJECT_NAME}
	source/main.cpp
)

set_target_properties(Vulkan PROPERTIES FOLDER ${ApplicationFolder})

#Includes
target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/source
)

#Libraries
target_link_libraries(
	${PROJECT_NAME}
	PUBLIC 
		
)