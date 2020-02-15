set(TNC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(TNC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Tunic)
set(TNC_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../res)

project(Tunic)

add_library(${PROJECT_NAME} STATIC)

set(
	ECSFiles
		${TNC_INCLUDE}/ECS/Core/Component.hpp
		${TNC_INCLUDE}/ECS/Core/Component.inl
		${TNC_INCLUDE}/ECS/Core/ComponentManager.hpp
		${TNC_INCLUDE}/ECS/Core/ComponentManager.inl
		${TNC_SOURCE}/ECS/Core/ComponentManager.cpp
		${TNC_INCLUDE}/ECS/Core/ECSTypes.hpp
		${TNC_INCLUDE}/ECS/Core/Entity.hpp
		${TNC_INCLUDE}/ECS/Core/Entity.inl
		${TNC_SOURCE}/ECS/Core/Entity.cpp
		${TNC_INCLUDE}/ECS/Core/World.hpp
		${TNC_INCLUDE}/ECS/Core/World.inl
		${TNC_SOURCE}/ECS/Core/World.cpp
		${TNC_INCLUDE}/ECS/Core/System.hpp
		${TNC_INCLUDE}/ECS/2D/Components/RigidBodyComponent.hpp
		${TNC_SOURCE}/ECS/2D/Components/RigidBodyComponent.cpp
		${TNC_INCLUDE}/ECS/2D/Components/SpriteComponent.hpp
		${TNC_SOURCE}/ECS/2D/Components/SpriteComponent.cpp
		${TNC_INCLUDE}/ECS/2D/Components/TransformComponent.hpp
		${TNC_SOURCE}/ECS/2D/Components/TransformComponent.cpp
		${TNC_INCLUDE}/ECS/2D/Systems/RenderSystem.hpp
		${TNC_SOURCE}/ECS/2D/Systems/RenderSystem.cpp
		${TNC_INCLUDE}/ECS/2D/Systems/PhysicsSystem.hpp
		${TNC_SOURCE}/ECS/2D/Systems/PhysicsSystem.cpp
		${TNC_INCLUDE}/ECS/3D/Components/CameraComponent.hpp
		${TNC_SOURCE}/ECS/3D/Components/CameraComponent.cpp
		${TNC_INCLUDE}/ECS/3D/Components/LightComponent.hpp
		${TNC_SOURCE}/ECS/3D/Components/LightComponent.cpp
		${TNC_INCLUDE}/ECS/3D/Components/MeshComponent.hpp
		${TNC_SOURCE}/ECS/3D/Components/MeshComponent.cpp
		${TNC_INCLUDE}/ECS/3D/Components/RigidBodyComponent.hpp
		${TNC_SOURCE}/ECS/3D/Components/RigidBodyComponent.cpp
		${TNC_INCLUDE}/ECS/3D/Components/TransformComponent.hpp
		${TNC_SOURCE}/ECS/3D/Components/TransformComponent.cpp
		${TNC_INCLUDE}/ECS/3D/Systems/RenderSystem.hpp
		${TNC_SOURCE}/ECS/3D/Systems/RenderSystem.cpp
		${TNC_INCLUDE}/ECS/3D/Systems/PhysicsSystem.hpp
		${TNC_SOURCE}/ECS/3D/Systems/PhysicsSystem.cpp
		${TNC_INCLUDE}/ECS/Audio/Components/AudioComponent.hpp
		${TNC_SOURCE}/ECS/Audio/Components/AudioComponent.cpp
		${TNC_INCLUDE}/ECS/Audio/Systems/AudioSystem.hpp
		${TNC_SOURCE}/ECS/Audio/Systems/AudioSystem.cpp
		${TNC_INCLUDE}/ECS/UI/Components/TextComponent.hpp
		${TNC_SOURCE}/ECS/UI/Components/TextComponent.cpp
		${TNC_INCLUDE}/ECS/UI/Components/TransformComponent.hpp
		${TNC_SOURCE}/ECS/UI/Components/TransformComponent.cpp
		${TNC_INCLUDE}/ECS/UI/Components/WidgetComponent.hpp
		${TNC_SOURCE}/ECS/UI/Components/WidgetComponent.cpp
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
		${TNC_INCLUDE}/Rendering/Renderables/Sprite.hpp
		${TNC_SOURCE}/Rendering/Renderables/Sprite.cpp
		${TNC_INCLUDE}/Rendering/Renderables/Text.hpp
		${TNC_SOURCE}/Rendering/Renderables/Text.cpp
)

set(
	ShaderFiles
		${TNC_RESOURCE}/Shaders/2D-ps.hlsl
		${TNC_RESOURCE}/Shaders/2D-vs.hlsl
		${TNC_RESOURCE}/Shaders/CubeShadowMap-gs.hlsl
		${TNC_RESOURCE}/Shaders/CubeShadowMap-ps.hlsl
		${TNC_RESOURCE}/Shaders/CubeShadowMap-vs.hlsl
		${TNC_RESOURCE}/Shaders/Font-ps.hlsl
		${TNC_RESOURCE}/Shaders/Font-vs.hlsl
		${TNC_RESOURCE}/Shaders/Lit-ps.hlsl
		${TNC_RESOURCE}/Shaders/Lit-vs.hlsl
		${TNC_RESOURCE}/Shaders/RT-ps.hlsl
		${TNC_RESOURCE}/Shaders/RT-vs.hlsl
		${TNC_RESOURCE}/Shaders/Unlit-ps.hlsl
		${TNC_RESOURCE}/Shaders/Unlit-vs.hlsl

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
        
		${ECSFiles}
		${RenderingFiles}
		${ShaderFiles}
)

#Set as header only so VS doesn't try to compile them
set_source_files_properties(${ShaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)