set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/AI)

#Library
add_library(
    CloveAI STATIC

    ${INCLUDE}/BehaviourTree.hpp
	${SOURCE}/BehaviourTree.cpp
	${INCLUDE}/BlackBoard.hpp
	${INCLUDE}/BlackBoard.inl
	${SOURCE}/BlackBoard.cpp
	${INCLUDE}/Composite.hpp
	${SOURCE}/Composite.cpp
	${INCLUDE}/Decorator.hpp
	${SOURCE}/Decorator.cpp
	${INCLUDE}/Selector.hpp
	${SOURCE}/Selector.cpp
	${INCLUDE}/Sequence.hpp
	${SOURCE}/Sequence.cpp
	${INCLUDE}/Task.hpp
)

#Includes
target_include_directories(
    CloveAI
    
	PUBLIC
		include

)

#Libraries
target_link_libraries(
    CloveAI

    PUBLIC
        CloveDefinitions
        CloveDeltaTime
        CloveLog
		CloveMemory
)

copy_target_includes_to_out_dir(CloveAI)