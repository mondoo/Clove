set(BLB_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(BLB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Bulb)
set(BLB_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../res)

project(Bulb)

add_library(${PROJECT_NAME} STATIC)

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
		${BLB_INCLUDE}/Audio/Sound.hpp
		${BLB_SOURCE}/Audio/Sound.cpp
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
		${BLB_INCLUDE}/ECS/Components/AudioComponent.hpp
		${BLB_SOURCE}/ECS/Components/AudioComponent.cpp
		${BLB_INCLUDE}/ECS/Components/CameraComponent.hpp
		${BLB_SOURCE}/ECS/Components/CameraComponent.cpp
		${BLB_INCLUDE}/ECS/Components/DirectionalLightComponent.hpp
		${BLB_SOURCE}/ECS/Components/DirectionalLightComponent.cpp
		${BLB_INCLUDE}/ECS/Components/ModelComponent.hpp
		${BLB_SOURCE}/ECS/Components/ModelComponent.cpp
		${BLB_INCLUDE}/ECS/Components/PointLightComponent.hpp
		${BLB_SOURCE}/ECS/Components/PointLightComponent.cpp
		${BLB_INCLUDE}/ECS/Components/RigidBodyComponent.hpp
		${BLB_SOURCE}/ECS/Components/RigidBodyComponent.cpp
		${BLB_INCLUDE}/ECS/Components/TransformComponent.hpp
		${BLB_SOURCE}/ECS/Components/TransformComponent.cpp
		${BLB_INCLUDE}/ECS/Systems/AudioSystem.hpp
		${BLB_SOURCE}/ECS/Systems/AudioSystem.cpp
		${BLB_INCLUDE}/ECS/Systems/RenderSystem.hpp
		${BLB_SOURCE}/ECS/Systems/RenderSystem.cpp
		${BLB_INCLUDE}/ECS/Systems/PhysicsSystem.hpp
		${BLB_SOURCE}/ECS/Systems/PhysicsSystem.cpp
)

set(
	PhysicsFiles
		${BLB_INCLUDE}/Physics/RigidBody.hpp
		${BLB_SOURCE}/Physics/RigidBody.cpp
		${BLB_INCLUDE}/Physics/World.hpp
		${BLB_SOURCE}/Physics/World.cpp
)

set(
	RenderingFiles
		${BLB_INCLUDE}/Rendering/AnimationTypes.hpp
		${BLB_INCLUDE}/Rendering/Animator.hpp
		${BLB_SOURCE}/Rendering/Animator.cpp
		${BLB_INCLUDE}/Rendering/Camera.hpp
		${BLB_SOURCE}/Rendering/Camera.cpp
		${BLB_INCLUDE}/Rendering/Material.hpp
		${BLB_INCLUDE}/Rendering/Material.inl
		${BLB_SOURCE}/Rendering/Material.cpp
		${BLB_INCLUDE}/Rendering/MaterialInstance.hpp
		${BLB_INCLUDE}/Rendering/MaterialInstance.inl
		${BLB_SOURCE}/Rendering/MaterialInstance.cpp
		${BLB_INCLUDE}/Rendering/RenderingConstants.hpp
		${BLB_INCLUDE}/Rendering/ShaderBufferTypes.hpp
		${BLB_INCLUDE}/Rendering/Renderer3D.hpp
		${BLB_SOURCE}/Rendering/Renderer3D.cpp
		${BLB_INCLUDE}/Rendering/Renderables/Font.hpp
		${BLB_SOURCE}/Rendering/Renderables/Font.cpp
		${BLB_INCLUDE}/Rendering/Renderables/Mesh.hpp
		${BLB_SOURCE}/Rendering/Renderables/Mesh.cpp
		${BLB_INCLUDE}/Rendering/Renderables/AnimatedModel.hpp
		${BLB_SOURCE}/Rendering/Renderables/AnimatedModel.cpp
		${BLB_INCLUDE}/Rendering/Renderables/StaticModel.hpp
		${BLB_SOURCE}/Rendering/Renderables/StaticModel.cpp
		${BLB_INCLUDE}/Rendering/Renderables/Sprite.hpp
		${BLB_SOURCE}/Rendering/Renderables/Sprite.cpp
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

set(
	UIFiles
		${BLB_INCLUDE}/UI/DrawableElement.hpp
		${BLB_INCLUDE}/UI/Image.hpp
		${BLB_SOURCE}/UI/Image.cpp
		${BLB_INCLUDE}/UI/InputElement.hpp
		${BLB_INCLUDE}/UI/Text.hpp
		${BLB_SOURCE}/UI/Text.cpp
		${BLB_INCLUDE}/UI/UIFrame.hpp
		${BLB_SOURCE}/UI/UIFrame.cpp
)

set(
	ShaderFiles
		${BLB_RESOURCE}/Shaders/Default2D-ps.hlsl
		${BLB_RESOURCE}/Shaders/Default2D-vs.hlsl
		${BLB_RESOURCE}/Shaders/Default3D-ps.hlsl
		${BLB_RESOURCE}/Shaders/Default3D-vs.hlsl
		${BLB_RESOURCE}/Shaders/Font-ps.hlsl
		${BLB_RESOURCE}/Shaders/Font-vs.hlsl
		${BLB_RESOURCE}/Shaders/GenCubeShadowMap-gs.hlsl
		${BLB_RESOURCE}/Shaders/GenCubeShadowMap-ps.hlsl
		${BLB_RESOURCE}/Shaders/GenCubeShadowMap-vs.hlsl
		${BLB_RESOURCE}/Shaders/GenShadowMap-gs.hlsl
		${BLB_RESOURCE}/Shaders/GenShadowMap-ps.hlsl
		${BLB_RESOURCE}/Shaders/GenShadowMap-vs.hlsl
		${BLB_RESOURCE}/Shaders/ShadowFunctions.hlsl
		${BLB_RESOURCE}/Shaders/Skeletal3D-vs.hlsl
		${BLB_RESOURCE}/Shaders/SkeletalCubeShadow-vs.hlsl
		${BLB_RESOURCE}/Shaders/SkeletalShadow-vs.hlsl
)

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

embeddResource("${ShaderFiles}" "r")
embeddResource("${BLB_RESOURCE}/Fonts/Roboto/Roboto-Black.ttf" "rb")

#Organise the source tree
file(GLOB_RECURSE header ${BLB_INCLUDE}/*.h ${BLB_INCLUDE}/*.hpp ${BLB_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm)
file(GLOB_RECURSE resource ${BLB_RESOURCE}/*.hlsl)
source_group(TREE ${BLB_INCLUDE} FILES ${header})
source_group(TREE ${BLB_SOURCE} FILES ${source})
source_group(TREE ${BLB_RESOURCE} FILES ${resource})

source_group(PCH FILES 
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/${PROJECT_NAME}.dir/cmake_pch.hxx
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/${PROJECT_NAME}.dir/cmake_pch.cxx
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${BLB_INCLUDE}/Bulb.hpp

		${BLB_INCLUDE}/InputResponse.hpp

		${BLB_INCLUDE}/Layer.hpp
		${BLB_SOURCE}/Layer.cpp
		${BLB_INCLUDE}/LayerStack.hpp
		${BLB_SOURCE}/LayerStack.cpp

		${BLB_INCLUDE}/ModelLoader.hpp
		${BLB_SOURCE}/ModelLoader.cpp
        
		${AIFiles}
		${AudioFiles}
		${ControllerFiles}
		${ECSFiles}
		${PhysicsFiles}
		${RenderingFiles}
		${StateMachineFiles}
		${UIFiles}
		${ShaderFiles}

		${ResourceBinaries}
)

#Set as header only so VS doesn't try to compile them
set_source_files_properties(${ShaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)