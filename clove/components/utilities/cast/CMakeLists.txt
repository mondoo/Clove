#Library
add_library(
    CloveCast INTERFACE
)

#Include
target_include_directories(
	CloveCast
	
	INTERFACE 
		include
)

#Libraries
target_link_libraries(
    CloveCast

    INTERFACE
        CloveDefinitions
        CloveLog
)