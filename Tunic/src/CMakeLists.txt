set(TNC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(TNC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Tunic)

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
		${TNC_INCLUDE}/ECS/Core/Manager.hpp
		${TNC_INCLUDE}/ECS/Core/Manager.inl
		${TNC_SOURCE}/ECS/Core/Manager.cpp
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

file(GLOB_RECURSE header ${TNC_INCLUDE}/*.h ${TNC_INCLUDE}/*.hpp ${TNC_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm *.glsl *.hlsl)
source_group(TREE ${TNC_INCLUDE} FILES ${header})
source_group(TREE ${TNC_SOURCE} FILES ${source})

target_sources(
	${PROJECT_NAME}
    PRIVATE
        ${TNC_SOURCE}/Application.cpp
        ${TNC_INCLUDE}/Application.hpp
        
		${ECSFiles}
)