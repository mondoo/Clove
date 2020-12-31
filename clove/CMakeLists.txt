set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove)
set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_subdirectory(components)
add_subdirectory(external)
add_subdirectory(tools)

#Prepare SOURCEs to be inbedded
set(
	ShaderIncludes
		${SOURCE}/Shaders/Constants.glsl
)

set(
	ShaderFiles
		${SOURCE}/Shaders/StaticMesh-v.glsl
		${SOURCE}/Shaders/StaticMeshShadowMap-v.glsl
		${SOURCE}/Shaders/StaticMeshCubeShadowMap-v.glsl

		${SOURCE}/Shaders/AnimatedMesh-v.glsl
		${SOURCE}/Shaders/AnimatedMeshShadowMap-v.glsl
		${SOURCE}/Shaders/AnimatedMeshCubeShadowMap-v.glsl

		${SOURCE}/Shaders/Mesh-p.glsl
		${SOURCE}/Shaders/MeshShadowMap-p.glsl
		${SOURCE}/Shaders/MeshCubeShadowMap-p.glsl

		${SOURCE}/Shaders/UI-v.glsl
		${SOURCE}/Shaders/Widget-p.glsl
		${SOURCE}/Shaders/Font-p.glsl

		${ShaderIncludes}
)

#Convert SOURCEs into c strings
set(ResourceBinaries)

function(embeddSOURCE SOURCEs readType)
	set(files)

	foreach(res IN ITEMS ${SOURCEs})
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

#Assets
embeddSOURCE("${ASSETS}/fonts/Roboto/Roboto-Black.ttf" "rb")

#Shader include
embeddSOURCE("${SOURCE}/Shaders/Constants.glsl" "r")

#Shader source
embeddSOURCE("${SOURCE}/Shaders/StaticMesh-v.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/StaticMeshShadowMap-v.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/StaticMeshCubeShadowMap-v.glsl" "r")

embeddSOURCE("${SOURCE}/Shaders/AnimatedMesh-v.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/AnimatedMeshShadowMap-v.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/AnimatedMeshCubeShadowMap-v.glsl" "r")

embeddSOURCE("${SOURCE}/Shaders/Mesh-p.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/MeshShadowMap-p.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/MeshCubeShadowMap-p.glsl" "r")

embeddSOURCE("${SOURCE}/Shaders/UI-v.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/Widget-p.glsl" "r")
embeddSOURCE("${SOURCE}/Shaders/Font-p.glsl" "r")

#Library
add_library(
	Clove STATIC

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
		${INCLUDE}/Components/CollisionShapeComponent.hpp
		${SOURCE}/Components/CollisionShapeComponent.cpp
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
		${INCLUDE}/Rendering/Camera.inl
		${SOURCE}/Rendering/Camera.cpp
		${INCLUDE}/Rendering/ForwardRenderer3D.hpp
		${SOURCE}/Rendering/ForwardRenderer3D.cpp
		${INCLUDE}/Rendering/GraphicsImageRenderTarget.hpp
		${SOURCE}/Rendering/GraphicsImageRenderTarget.cpp
		${INCLUDE}/Rendering/Material.hpp
		${INCLUDE}/Rendering/Material.inl
		${SOURCE}/Rendering/Material.cpp
		${INCLUDE}/Rendering/RenderingConstants.hpp
		${INCLUDE}/Rendering/RenderingHelpers.hpp
		${SOURCE}/Rendering/RenderingHelpers.cpp
		${INCLUDE}/Rendering/RenderTarget.hpp
		${INCLUDE}/Rendering/SwapchainRenderTarget.hpp
		${SOURCE}/Rendering/SwapchainRenderTarget.cpp
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

		${INCLUDE}/Layers/AudioLayer.hpp
		${SOURCE}/Layers/AudioLayer.cpp
		${INCLUDE}/Layers/RenderLayer.hpp
		${SOURCE}/Layers/RenderLayer.cpp
		${INCLUDE}/Layers/PhysicsLayer.hpp
		${SOURCE}/Layers/PhysicsLayer.cpp

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

		${INCLUDE}/Application.hpp
		${INCLUDE}/Application.inl
		${SOURCE}/Application.cpp

		${INCLUDE}/InputEvent.hpp
		${INCLUDE}/InputResponse.hpp

		${INCLUDE}/Layer.hpp
		${INCLUDE}/Layer.inl
		${SOURCE}/Layer.cpp
		
		${INCLUDE}/ModelLoader.hpp
		${SOURCE}/ModelLoader.cpp

		${INCLUDE}/SoundFile.hpp
		${SOURCE}/SoundFile.cpp

		${INCLUDE}/TextureLoader.hpp
		${SOURCE}/TextureLoader.cpp

		${ShaderFiles}

		${ResourceBinaries}
)

#Set as header only so VS doesn't try to compile them
set_source_files_properties(${ShaderFiles} PROPERTIES HEADER_FILE_ONLY TRUE)

#Includes
target_include_directories(
	Clove

	PUBLIC
		include

	PRIVATE
		external/freetype2/include
		external/bullet3/src
)

#Libraries
target_link_libraries(
	Clove

	#Components 
	PUBLIC 
		#Core 
		CloveAudio 
		CloveGraphics 
		ClovePlatform 
 
		#Systems
		CloveAI
		CloveECS 
		CloveStateMachine
		 
		#Utilities 
		CloveCast 
		CloveDefinitions 
		CloveDelegate 
		CloveDeltaTime 
		CloveEvent 
		CloveExpected 
		CloveLog 
		CloveMaths 
		CloveMemory 
		CloveTimer

	PRIVATE
		#Order of these are important
		BulletDynamics
		BulletCollision
		LinearMath

		assimp
		stb
		freetype
		sndfile
)