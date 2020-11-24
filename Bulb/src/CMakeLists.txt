set(BLB_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(BLB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Bulb)
set(BLB_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../res)

include(../../Clove/cmake/CompileShader.cmake)

add_library(Bulb STATIC)

set(
	ControllerFiles
		${BLB_INCLUDE}/Controller/Controller.hpp
		${BLB_INCLUDE}/Controller/BTAIController.hpp
		${BLB_SOURCE}/Controller/BTAIController.cpp
		${BLB_INCLUDE}/Controller/PlayerController.hpp
)

set(
	StateMachineFiles
		${BLB_INCLUDE}/StateMachine/State.hpp
		${BLB_INCLUDE}/StateMachine/State.inl
		${BLB_INCLUDE}/StateMachine/StateMachine.hpp
		${BLB_INCLUDE}/StateMachine/StateMachine.inl
		${BLB_INCLUDE}/StateMachine/Transition.hpp
		${BLB_INCLUDE}/StateMachine/Transition.inl
)

#Shader files that are only used for includes


#Organise the source tree
file(GLOB_RECURSE header ${BLB_INCLUDE}/*.h ${BLB_INCLUDE}/*.hpp ${BLB_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm)
file(GLOB_RECURSE resource ${BLB_RESOURCE}/*.glsl)
source_group(TREE ${BLB_INCLUDE} FILES ${header})
source_group(TREE ${BLB_SOURCE} FILES ${source})
source_group(TREE ${BLB_RESOURCE} FILES ${resource})

source_group(PCH FILES 
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/Bulb.dir/cmake_pch.hxx
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/Bulb.dir/cmake_pch.cxx
)

target_sources(
	Bulb
	PRIVATE
		${BLB_INCLUDE}/Bulb.hpp

		${ControllerFiles}
		${StateMachineFiles}
)