set(BLB_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(BLB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Bulb)
set(BLB_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../res)

include(../../Clove/cmake/CompileShader.cmake)

add_library(Bulb STATIC)

set(
	AIFiles
		${BLB_INCLUDE}/AI/BehaviourTree.hpp
		${BLB_SOURCE}/AI/BehaviourTree.cpp
		${BLB_INCLUDE}/AI/BlackBoard.hpp
		${BLB_INCLUDE}/AI/BlackBoard.inl
		${BLB_SOURCE}/AI/BlackBoard.cpp
		${BLB_INCLUDE}/AI/Composite.hpp
		${BLB_SOURCE}/AI/Composite.cpp
		${BLB_INCLUDE}/AI/Decorator.hpp
		${BLB_SOURCE}/AI/Decorator.cpp
		${BLB_INCLUDE}/AI/Selector.hpp
		${BLB_SOURCE}/AI/Selector.cpp
		${BLB_INCLUDE}/AI/Sequence.hpp
		${BLB_SOURCE}/AI/Sequence.cpp
		${BLB_INCLUDE}/AI/Task.hpp
)

set(
	AudioFiles
		${BLB_INCLUDE}/Audio/SoundFile.hpp
		${BLB_SOURCE}/Audio/SoundFile.cpp
)

set(
	ControllerFiles
		${BLB_INCLUDE}/Controller/Controller.hpp
		${BLB_INCLUDE}/Controller/BTAIController.hpp
		${BLB_SOURCE}/Controller/BTAIController.cpp
		${BLB_INCLUDE}/Controller/PlayerController.hpp
)

set(
	ECSFiles
		${BLB_INCLUDE}/ECS/Component.hpp
		${BLB_INCLUDE}/ECS/Component.inl
		${BLB_INCLUDE}/ECS/ComponentManager.hpp
		${BLB_INCLUDE}/ECS/ComponentManager.inl
		${BLB_INCLUDE}/ECS/ComponentSet.hpp
		${BLB_SOURCE}/ECS/ComponentManager.cpp
		${BLB_INCLUDE}/ECS/ECSEvents.hpp
		${BLB_INCLUDE}/ECS/ECSTypes.hpp
		${BLB_INCLUDE}/ECS/Entity.hpp
		${BLB_INCLUDE}/ECS/Entity.inl
		${BLB_SOURCE}/ECS/Entity.cpp
		${BLB_INCLUDE}/ECS/World.hpp
		${BLB_INCLUDE}/ECS/World.inl
		${BLB_SOURCE}/ECS/World.cpp
		${BLB_INCLUDE}/ECS/System.hpp
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
set(
	ShaderIncludes
		${BLB_RESOURCE}/Shaders/Constants.glsl
)

set(
	ShaderFiles
		${BLB_RESOURCE}/Shaders/StaticMesh-v.glsl
		${BLB_RESOURCE}/Shaders/StaticMeshShadowMap-v.glsl
		${BLB_RESOURCE}/Shaders/StaticMeshCubeShadowMap-v.glsl

		${BLB_RESOURCE}/Shaders/AnimatedMesh-v.glsl
		${BLB_RESOURCE}/Shaders/AnimatedMeshShadowMap-v.glsl
		${BLB_RESOURCE}/Shaders/AnimatedMeshCubeShadowMap-v.glsl

		${BLB_RESOURCE}/Shaders/Mesh-p.glsl
		${BLB_RESOURCE}/Shaders/MeshShadowMap-p.glsl
		${BLB_RESOURCE}/Shaders/MeshCubeShadowMap-p.glsl

		${BLB_RESOURCE}/Shaders/UI-v.glsl
		${BLB_RESOURCE}/Shaders/Widget-p.glsl
		${BLB_RESOURCE}/Shaders/Font-p.glsl

		${ShaderIncludes}
)

compileShader("${BLB_RESOURCE}/Shaders/StaticMesh-v.glsl" "StaticMesh-v.spirv" "vertex" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/StaticMeshShadowMap-v.glsl" "StaticMeshShadowMap-v.spirv" "vertex" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/StaticMeshCubeShadowMap-v.glsl" "StaticMeshCubeShadowMap-v.spirv" "vertex" "${ShaderIncludes}")

compileShader("${BLB_RESOURCE}/Shaders/AnimatedMesh-v.glsl" "AnimatedMesh-v.spirv" "vertex" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/AnimatedMeshShadowMap-v.glsl" "AnimatedMeshShadowMap-v.spirv" "vertex" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/AnimatedMeshCubeShadowMap-v.glsl" "AnimatedMeshCubeShadowMap-v.spirv" "vertex" "${ShaderIncludes}")

compileShader("${BLB_RESOURCE}/Shaders/Mesh-p.glsl" "Mesh-p.spirv" "fragment" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/MeshShadowMap-p.glsl" "MeshShadowMap-p.spirv" "fragment" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/MeshCubeShadowMap-p.glsl" "MeshCubeShadowMap-p.spirv" "fragment" "${ShaderIncludes}")

compileShader("${BLB_RESOURCE}/Shaders/UI-v.glsl" "UI-v.spirv" "vertex" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/Widget-p.glsl" "Widget-p.spirv" "fragment" "${ShaderIncludes}")
compileShader("${BLB_RESOURCE}/Shaders/Font-p.glsl" "Font-p.spirv" "fragment" "${ShaderIncludes}")

#Convert resources into c strings
set(ResourceBinaries)

function(embeddResource resources readType)
	set(files)

	foreach(res IN ITEMS ${resources})
		get_filename_component(fileName ${res} NAME_WLE)

		string(REPLACE "-" "_" fileName ${fileName})
		string(TOLOWER ${fileName} fileName)

		add_custom_command(
			OUTPUT ${fileName}.c
			COMMAND ResourceEmbedder ${fileName} ${res} ${readType}
			DEPENDS ${res}
		)

		set(files ${files} "${fileName}.c")
	endforeach()

	set(ResourceBinaries ${ResourceBinaries} ${files} PARENT_SCOPE)
endfunction()

embeddResource("${BLB_RESOURCE}/Fonts/Roboto/Roboto-Black.ttf" "rb")

embeddResource("StaticMesh-v.spirv" "rb")
embeddResource("StaticMeshShadowMap-v.spirv" "rb")
embeddResource("StaticMeshCubeShadowMap-v.spirv" "rb")

embeddResource("AnimatedMesh-v.spirv" "rb")
embeddResource("AnimatedMeshShadowMap-v.spirv" "rb")
embeddResource("AnimatedMeshCubeShadowMap-v.spirv" "rb")

embeddResource("Mesh-p.spirv" "rb")
embeddResource("MeshShadowMap-p.spirv" "rb")
embeddResource("MeshCubeShadowMap-p.spirv" "rb")

embeddResource("UI-v.spirv" "rb")
embeddResource("Widget-p.spirv" "rb")
embeddResource("Font-p.spirv" "rb")

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

		${BLB_INCLUDE}/InputResponse.hpp
        
		${AIFiles}
		${AudioFiles}
		${ControllerFiles}
		${ECSFiles}
		${StateMachineFiles}

		${ShaderFiles}

		${ResourceBinaries}
)

#Set as header only so VS doesn't try to compile them
set_source_files_properties(${ShaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)