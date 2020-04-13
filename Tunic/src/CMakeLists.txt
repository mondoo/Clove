set(TNC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(TNC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Tunic)
set(TNC_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../res)

project(Tunic)

add_library(${PROJECT_NAME} STATIC)

set(
	ECSFiles
		${TNC_INCLUDE}/ECS/Component.hpp
		${TNC_INCLUDE}/ECS/Component.inl
		${TNC_INCLUDE}/ECS/ComponentManager.hpp
		${TNC_INCLUDE}/ECS/ComponentManager.inl
		${TNC_SOURCE}/ECS/ComponentManager.cpp
		${TNC_INCLUDE}/ECS/ECSTypes.hpp
		${TNC_INCLUDE}/ECS/Entity.hpp
		${TNC_INCLUDE}/ECS/Entity.inl
		${TNC_SOURCE}/ECS/Entity.cpp
		${TNC_INCLUDE}/ECS/World.hpp
		${TNC_INCLUDE}/ECS/World.inl
		${TNC_SOURCE}/ECS/World.cpp
		${TNC_INCLUDE}/ECS/System.hpp
		${TNC_INCLUDE}/ECS/Components/CameraComponent.hpp
		${TNC_SOURCE}/ECS/Components/CameraComponent.cpp
		${TNC_INCLUDE}/ECS/Components/DirectionalLightComponent.hpp
		${TNC_SOURCE}/ECS/Components/DirectionalLightComponent.cpp
		${TNC_INCLUDE}/ECS/Components/ModelComponent.hpp
		${TNC_SOURCE}/ECS/Components/ModelComponent.cpp
		${TNC_INCLUDE}/ECS/Components/PointLightComponent.hpp
		${TNC_SOURCE}/ECS/Components/PointLightComponent.cpp
		${TNC_INCLUDE}/ECS/Components/RigidBodyComponent.hpp
		${TNC_SOURCE}/ECS/Components/RigidBodyComponent.cpp
		${TNC_INCLUDE}/ECS/Components/TransformComponent.hpp
		${TNC_SOURCE}/ECS/Components/TransformComponent.cpp
		${TNC_INCLUDE}/ECS/Systems/RenderSystem.hpp
		${TNC_SOURCE}/ECS/Systems/RenderSystem.cpp
		${TNC_INCLUDE}/ECS/Systems/PhysicsSystem.hpp
		${TNC_SOURCE}/ECS/Systems/PhysicsSystem.cpp
		${TNC_INCLUDE}/ECS/Components/AudioComponent.hpp
		${TNC_SOURCE}/ECS/Components/AudioComponent.cpp
		${TNC_INCLUDE}/ECS/Systems/AudioSystem.hpp
		${TNC_SOURCE}/ECS/Systems/AudioSystem.cpp
)

set(
	PhysicsFiles
		${TNC_INCLUDE}/Physics/RigidBody.hpp
		${TNC_SOURCE}/Physics/RigidBody.cpp
		${TNC_INCLUDE}/Physics/World.hpp
		${TNC_SOURCE}/Physics/World.cpp
)

set(
	RenderingFiles
		${TNC_INCLUDE}/Rendering/Material.hpp
		${TNC_INCLUDE}/Rendering/Material.inl
		${TNC_SOURCE}/Rendering/Material.cpp
		${TNC_INCLUDE}/Rendering/MaterialInstance.hpp
		${TNC_INCLUDE}/Rendering/MaterialInstance.inl
		${TNC_SOURCE}/Rendering/MaterialInstance.cpp
		${TNC_INCLUDE}/Rendering/RenderingConstants.hpp
		${TNC_INCLUDE}/Rendering/ShaderBufferTypes.hpp
		${TNC_INCLUDE}/Rendering/Renderer.hpp
		${TNC_SOURCE}/Rendering/Renderer.cpp
		${TNC_INCLUDE}/Rendering/Renderer2D.hpp
		${TNC_SOURCE}/Rendering/Renderer2D.cpp
		${TNC_INCLUDE}/Rendering/Renderables/Font.hpp
		${TNC_SOURCE}/Rendering/Renderables/Font.cpp
		${TNC_INCLUDE}/Rendering/Renderables/Mesh.hpp
		${TNC_SOURCE}/Rendering/Renderables/Mesh.cpp
		${TNC_INCLUDE}/Rendering/Renderables/Model.hpp
		${TNC_SOURCE}/Rendering/Renderables/Model.cpp
		${TNC_INCLUDE}/Rendering/Renderables/Sprite.hpp
		${TNC_SOURCE}/Rendering/Renderables/Sprite.cpp
		
)

set(
	UIFiles
		${TNC_INCLUDE}/UI/Image.hpp
		${TNC_SOURCE}/UI/Image.cpp
		${TNC_INCLUDE}/UI/Text.hpp
		${TNC_SOURCE}/UI/Text.cpp
		${TNC_INCLUDE}/UI/Widget.hpp
		${TNC_SOURCE}/UI/Widget.cpp
)

set(
	ShaderFiles
		${TNC_RESOURCE}/Shaders/Default2D-ps.hlsl
		${TNC_RESOURCE}/Shaders/Default2D-vs.hlsl
		${TNC_RESOURCE}/Shaders/Default3D-ps.hlsl
		${TNC_RESOURCE}/Shaders/Default3D-vs.hlsl
		${TNC_RESOURCE}/Shaders/Font-ps.hlsl
		${TNC_RESOURCE}/Shaders/Font-vs.hlsl
		${TNC_RESOURCE}/Shaders/GenCubeShadowMap-gs.hlsl
		${TNC_RESOURCE}/Shaders/GenCubeShadowMap-ps.hlsl
		${TNC_RESOURCE}/Shaders/GenCubeShadowMap-vs.hlsl
		${TNC_RESOURCE}/Shaders/GenShadowMap-gs.hlsl
		${TNC_RESOURCE}/Shaders/GenShadowMap-ps.hlsl
		${TNC_RESOURCE}/Shaders/GenShadowMap-vs.hlsl
		
)

file(GLOB_RECURSE header ${TNC_INCLUDE}/*.h ${TNC_INCLUDE}/*.hpp ${TNC_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm *.glsl *.hlsl)
file(GLOB_RECURSE resource ${TNC_RESOURCE}/*.hlsl)
source_group(TREE ${TNC_INCLUDE} FILES ${header})
source_group(TREE ${TNC_SOURCE} FILES ${source})
source_group(TREE ${TNC_RESOURCE} FILES ${resource})

target_sources(
	${PROJECT_NAME}
	PRIVATE
		${TNC_INCLUDE}/Tunic.hpp

        ${TNC_SOURCE}/Application.cpp
		${TNC_INCLUDE}/Application.hpp
		
		${TNC_INCLUDE}/Input.hpp
		${TNC_SOURCE}/Input.cpp

		${TNC_INCLUDE}/Layer.hpp
		${TNC_SOURCE}/Layer.cpp
		${TNC_INCLUDE}/LayerStack.hpp
		${TNC_SOURCE}/LayerStack.cpp

		${TNC_INCLUDE}/ModelLoader.hpp
		${TNC_SOURCE}/ModelLoader.cpp
        
		${ECSFiles}
		${PhysicsFiles}
		${RenderingFiles}
		${UIFiles}
		${ShaderFiles}
)

#Set as header only so VS doesn't try to compile them
set_source_files_properties(${ShaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)