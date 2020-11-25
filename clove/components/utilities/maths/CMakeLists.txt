add_subdirectory(external)

#Library
add_library(
    CloveMaths INTERFACE
)

#Include
target_include_directories(
	CloveMaths
	
	INTERFACE 
		include
)

#Libraries
target_link_libraries(
	CloveMaths

	INTERFACE
		glm
)

#Definitions
target_compile_definitions( 
	CloveMaths 
 
	INTERFACE 
		GLM_FORCE_RADIANS 
		GLM_FORCE_LEFT_HANDED 
		GLM_FORCE_DEPTH_ZERO_TO_ONE 
)