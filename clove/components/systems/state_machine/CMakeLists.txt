set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/StateMachine)

#Library
add_library(
    CloveStateMachine INTERFACE
)

#Includes
target_include_directories(
    CloveStateMachine
    
	INTERFACE
		include
)