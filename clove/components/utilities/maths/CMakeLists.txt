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