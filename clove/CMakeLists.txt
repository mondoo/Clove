set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove)
set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_subdirectory(components)
add_subdirectory(external)

#CMake
find_package(assimp REQUIRED CONFIG)
find_package(Bullet REQUIRED CONFIG)
find_package(Freetype REQUIRED CONFIG)
find_package(SndFile REQUIRED CONFIG)

#Prepare SOURCEs to be inbedded
set(
	ShaderIncludes
		${SOURCE}/Shaders/Constants.glsl
)

set(
	ShaderFiles
		${SOURCE}/Shaders/ForwardPassVert.glsl
		${SOURCE}/Shaders/ForwardPassPixel.glsl

		${SOURCE}/Shaders/DirShadowDepthsVert.glsl
		${SOURCE}/Shaders/DirShadowDepthsPixel.glsl
		
		${SOURCE}/Shaders/PointShadowDepthsVert.glsl
		${SOURCE}/Shaders/PointShadowDepthsPixel.glsl

		${SOURCE}/Shaders/UIVert.glsl
		${SOURCE}/Shaders/WidgetPixel.glsl
		${SOURCE}/Shaders/FontPixel.glsl

		${SOURCE}/Shaders/SkinningCompute.glsl

		${ShaderIncludes}
)

#Convert sourcess into c strings
set(ResourceBinaries)

function(embedSource source readType)
	set(files)

	get_filename_component(fileName ${source} NAME_WLE)

	string(REPLACE "-" "_" fileName ${fileName})
	string(TOLOWER ${fileName} fileName)

	add_custom_command(
		OUTPUT ${fileName}.c
		COMMAND reb ${fileName} ${source} ${readType}
		DEPENDS ${source}
	)

	set(files ${files} "${fileName}.c")

	set(ResourceBinaries ${ResourceBinaries} ${files} PARENT_SCOPE)
endfunction()

#Assets
embedSource("${ASSETS}/fonts/Roboto/Roboto-Black.ttf" "rb")

#Shaders
foreach(shader ${ShaderFiles})
	embedSource(${shader} "r")
endforeach()

#Library
add_library(
	Clove STATIC

		${INCLUDE}/Components/AnimatedModelComponent.hpp
		${INCLUDE}/Components/AudioListenerComponent.hpp
		${INCLUDE}/Components/AudioSourceComponent.hpp
		${INCLUDE}/Components/CameraComponent.hpp
		${SOURCE}/Components/CameraComponent.cpp
		${INCLUDE}/Components/CollisionResponseComponent.hpp
		${INCLUDE}/Components/CollisionShapeComponent.hpp
		${SOURCE}/Components/CollisionShapeComponent.cpp
		${INCLUDE}/Components/DirectionalLightComponent.hpp
		${INCLUDE}/Components/ParentComponent.hpp
		${INCLUDE}/Components/PointLightComponent.hpp
		${SOURCE}/Components/PointLightComponent.cpp
		${INCLUDE}/Components/RigidBodyComponent.hpp
		${SOURCE}/Components/RigidBodyComponent.cpp
		${INCLUDE}/Components/StaticModelComponent.hpp
		${SOURCE}/Components/StaticModelComponent.cpp
		${INCLUDE}/Components/TransformComponent.hpp
		${SOURCE}/Components/TransformComponent.cpp

		${INCLUDE}/FileSystem/AssetPtr.hpp
		${INCLUDE}/FileSystem/AssetPtr.inl
		${INCLUDE}/FileSystem/AssetManager.hpp
		${SOURCE}/FileSystem/AssetManager.cpp
		${INCLUDE}/FileSystem/VirtualFileSystem.hpp
		${SOURCE}/FileSystem/VirtualFileSystem.cpp

		${INCLUDE}/Rendering/AnimationTypes.hpp
		${INCLUDE}/Rendering/Animator.hpp
		${SOURCE}/Rendering/Animator.cpp
		${INCLUDE}/Rendering/Camera.hpp
		${INCLUDE}/Rendering/Camera.inl
		${SOURCE}/Rendering/Camera.cpp
		${INCLUDE}/Rendering/GraphicsImageRenderTarget.hpp
		${SOURCE}/Rendering/GraphicsImageRenderTarget.cpp
		${INCLUDE}/Rendering/HighDefinitionRenderer.hpp
		${SOURCE}/Rendering/HighDefinitionRenderer.cpp
		${INCLUDE}/Rendering/Material.hpp
		${INCLUDE}/Rendering/Material.inl
		${SOURCE}/Rendering/Material.cpp
		${INCLUDE}/Rendering/Renderer.hpp
		${INCLUDE}/Rendering/RenderingConstants.hpp
		${INCLUDE}/Rendering/RenderingHelpers.hpp
		${SOURCE}/Rendering/RenderingHelpers.cpp
		${INCLUDE}/Rendering/RenderingLog.hpp
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
		${INCLUDE}/Rendering/Renderables/Texture.hpp
		${INCLUDE}/Rendering/Renderables/Texture.inl
		${SOURCE}/Rendering/Renderables/Texture.cpp

		${INCLUDE}/Rendering/RenderGraph/CacheUtils.hpp
		${INCLUDE}/Rendering/RenderGraph/CacheUtils.inl
		${INCLUDE}/Rendering/RenderGraph/RenderGraph.hpp
		${SOURCE}/Rendering/RenderGraph/RenderGraph.cpp
		${INCLUDE}/Rendering/RenderGraph/RenderGraphLog.hpp
		${INCLUDE}/Rendering/RenderGraph/RgBindings.hpp
		${INCLUDE}/Rendering/RenderGraph/RgBuffer.hpp
		${INCLUDE}/Rendering/RenderGraph/RgBuffer.inl
		${SOURCE}/Rendering/RenderGraph/RgBuffer.cpp
		${INCLUDE}/Rendering/RenderGraph/RgComputePass.hpp
		${INCLUDE}/Rendering/RenderGraph/RgComputePass.inl
		${SOURCE}/Rendering/RenderGraph/RgComputePass.cpp
		${INCLUDE}/Rendering/RenderGraph/RgFrameCache.hpp
		${SOURCE}/Rendering/RenderGraph/RgFrameCache.cpp
		${INCLUDE}/Rendering/RenderGraph/RgGlobalCache.hpp
		${SOURCE}/Rendering/RenderGraph/RgGlobalCache.cpp
		${INCLUDE}/Rendering/RenderGraph/RgId.hpp
		${INCLUDE}/Rendering/RenderGraph/RgImage.hpp
		${INCLUDE}/Rendering/RenderGraph/RgImage.inl
		${SOURCE}/Rendering/RenderGraph/RgImage.cpp
		${INCLUDE}/Rendering/RenderGraph/RgPass.hpp
		${INCLUDE}/Rendering/RenderGraph/RgRenderPass.hpp
		${INCLUDE}/Rendering/RenderGraph/RgRenderPass.inl
		${SOURCE}/Rendering/RenderGraph/RgRenderPass.cpp
		${INCLUDE}/Rendering/RenderGraph/RgResource.hpp
		${INCLUDE}/Rendering/RenderGraph/RgResource.inl
		${SOURCE}/Rendering/RenderGraph/RgResource.cpp
		${INCLUDE}/Rendering/RenderGraph/RgSampler.hpp
		${INCLUDE}/Rendering/RenderGraph/RgShader.hpp
		${INCLUDE}/Rendering/RenderGraph/RgTransferPass.hpp
		${INCLUDE}/Rendering/RenderGraph/RgTransferPass.inl
		${SOURCE}/Rendering/RenderGraph/RgTransferPass.cpp
		${INCLUDE}/Rendering/RenderGraph/RgTypes.hpp
		
		${INCLUDE}/SubSystems/AudioSubSystem.hpp
		${SOURCE}/SubSystems/AudioSubSystem.cpp
		${INCLUDE}/SubSystems/PhysicsSubSystem.hpp
		${SOURCE}/SubSystems/PhysicsSubSystem.cpp
		${INCLUDE}/SubSystems/RenderSubSystem.hpp
		${SOURCE}/SubSystems/RenderSubSystem.cpp
		${INCLUDE}/SubSystems/TransformSubSystem.hpp
		${SOURCE}/SubSystems/TransformSubSystem.cpp

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

		${INCLUDE}/SubSystem.hpp
		${INCLUDE}/SubSystem.inl
		
		${INCLUDE}/ModelLoader.hpp
		${SOURCE}/ModelLoader.cpp

		${SOURCE}/ReflectionCommon.cpp

		${INCLUDE}/ReflectionAttributes.hpp

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
		CloveReflection
		CloveSerialisation
		CloveStateMachine
		 
		#Utilities
		CloveBlackboard
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
		assimp::assimp
		Bullet::Bullet
		Freetype::Freetype
		SndFile::SndFile
		stb
)

#Preprocessor
target_compile_definitions(
	Clove

	PUBLIC
		CLOVE_VERSION="${CMAKE_PROJECT_VERSION}"
)

if(CLOVE_BUILD_TESTS)
	add_subdirectory(tests)
endif()
