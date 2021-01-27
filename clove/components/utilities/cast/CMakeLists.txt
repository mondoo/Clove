#CMake
option(CLOVE_USE_SAFE_CAST "When set to ON polyCast will dynamic_cast instead of static. Useful in debug builds to catch any errors" OFF)

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
        CloveLog
)

#Preprocessor
target_compile_definitions(
	CloveCast

	INTERFACE
		CLOVE_SAFE_CAST=$<BOOL:${CLOVE_USE_SAFE_CAST}>
)