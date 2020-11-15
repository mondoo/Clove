set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Stem)

set(ApplicationFolder "Garlic/Stem")

add_subdirectory(external)

#Stem
add_library(
	Stem STATIC
		${INCLUDE}/EntryPoint.hpp

		${INCLUDE}/Application.hpp
		${SOURCE}/Application.cpp

		${INCLUDE}/Components/AnimatedModelComponent.hpp
		${INCLUDE}/Components/AnimatedModelComponent.inl
		${SOURCE}/Components/AnimatedModelComponent.cpp
		${INCLUDE}/Components/AudioListenerComponent.hpp
		${SOURCE}/Components/AudioListenerComponent.cpp
		${INCLUDE}/Components/AudioSourceComponent.hpp
		${SOURCE}/Components/AudioSourceComponent.cpp
		${INCLUDE}/Components/CameraComponent.hpp
		${INCLUDE}/Components/CameraComponent.inl
		${SOURCE}/Components/CameraComponent.cpp
		${INCLUDE}/Components/CollisionResponseComponent.hpp
		${INCLUDE}/Components/CubeColliderComponent.hpp
		${SOURCE}/Components/CubeColliderComponent.cpp
		${INCLUDE}/Components/DirectionalLightComponent.hpp
		${INCLUDE}/Components/DirectionalLightComponent.inl
		${SOURCE}/Components/DirectionalLightComponent.cpp
		${INCLUDE}/Components/PointLightComponent.hpp
		${INCLUDE}/Components/PointLightComponent.inl
		${SOURCE}/Components/PointLightComponent.cpp
		${INCLUDE}/Components/RigidBodyComponent.hpp
		${SOURCE}/Components/RigidBodyComponent.cpp
		${INCLUDE}/Components/StaticModelComponent.hpp
		${INCLUDE}/Components/StaticModelComponent.inl
		${SOURCE}/Components/StaticModelComponent.cpp
		${INCLUDE}/Components/TransformComponent.hpp
		${INCLUDE}/Components/TransformComponent.inl
		${SOURCE}/Components/TransformComponent.cpp

		${INCLUDE}/Rendering/AnimationTypes.hpp
		${INCLUDE}/Rendering/Animator.hpp
		${SOURCE}/Rendering/Animator.cpp
		${INCLUDE}/Rendering/Camera.hpp
		${SOURCE}/Rendering/Camera.cpp
		${INCLUDE}/Rendering/ForwardRenderer3D.hpp
		${SOURCE}/Rendering/ForwardRenderer3D.cpp
		${INCLUDE}/Rendering/Material.hpp
		${INCLUDE}/Rendering/Material.inl
		${SOURCE}/Rendering/Material.cpp
		${INCLUDE}/Rendering/RenderingConstants.hpp
		${INCLUDE}/Rendering/RenderingHelpers.hpp
		${SOURCE}/Rendering/RenderingHelpers.cpp
		${INCLUDE}/Rendering/ShaderBufferTypes.hpp
		${INCLUDE}/Rendering/Vertex.hpp
		${INCLUDE}/Rendering/Renderables/Font.hpp
		${SOURCE}/Rendering/Renderables/Font.cpp
		${INCLUDE}/Rendering/Renderables/Mesh.hpp
		${INCLUDE}/Rendering/Renderables/Mesh.inl
		${SOURCE}/Rendering/Renderables/Mesh.cpp
		${INCLUDE}/Rendering/Renderables/AnimatedModel.hpp
		${INCLUDE}/Rendering/Renderables/AnimatedModel.inl
		${SOURCE}/Rendering/Renderables/AnimatedModel.cpp
		${INCLUDE}/Rendering/Renderables/StaticModel.hpp
		${INCLUDE}/Rendering/Renderables/StaticModel.inl
		${SOURCE}/Rendering/Renderables/StaticModel.cpp

		${INCLUDE}/Systems/AudioSystem.hpp
		${SOURCE}/Systems/AudioSystem.cpp
		${INCLUDE}/Systems/RenderSystem.hpp
		${SOURCE}/Systems/RenderSystem.cpp
		${INCLUDE}/Systems/PhysicsSystem.hpp
		${SOURCE}/Systems/PhysicsSystem.cpp

		${INCLUDE}/UI/DrawableElement.hpp
		${INCLUDE}/UI/Image.hpp
		${INCLUDE}/UI/Image.inl
		${SOURCE}/UI/Image.cpp
		${INCLUDE}/UI/InputElement.hpp
		${INCLUDE}/UI/Text.hpp
		${INCLUDE}/UI/Text.inl
		${SOURCE}/UI/Text.cpp
		${INCLUDE}/UI/UIFrame.hpp
		${SOURCE}/UI/UIFrame.cpp
		
		${INCLUDE}/ModelLoader.hpp
		${SOURCE}/ModelLoader.cpp

		${INCLUDE}/TextureLoader.hpp
		${SOURCE}/TextureLoader.cpp
)

set_target_properties(Stem PROPERTIES FOLDER ${ApplicationFolder})

#Includes
target_include_directories(
	Stem
	PUBLIC
		include

	PRIVATE
		external/freetype2/include
		external/bullet3/src
)

#Libraries
target_link_libraries(
	Stem 
	PUBLIC 
		Bulb

	PRIVATE
		#Order of these are important
		BulletDynamics
		BulletCollision
		LinearMath

		assimp
		stb
		freetype
)