#Clove CMake list

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files
set(
	CoreFiles
		Clove.hpp
		Core/Core.hpp
		Core/IntTypes.hpp
		Core/Layer.hpp
		Core/Layer.cpp
		Core/LayerStack.hpp
		Core/LayerStack.cpp
		Core/Log.hpp
		Core/Log.cpp
)

set(
	CoreAudioFiles
		Core/Audio/Sound.hpp
		Core/Audio/Sound.cpp
)

set(
	CoreECSFiles
		Core/ECS/Component.hpp
		Core/ECS/Component.inl
		Core/ECS/ComponentManager.hpp
		Core/ECS/ComponentManager.inl
		Core/ECS/ComponentManager.cpp
		Core/ECS/ECSTypes.hpp
		Core/ECS/Entity.hpp
		Core/ECS/Entity.inl
		Core/ECS/Entity.cpp
		Core/ECS/Manager.hpp
		Core/ECS/Manager.inl
		Core/ECS/Manager.cpp
		Core/ECS/System.hpp
		Core/ECS/2D/Components/RigidBodyComponent.hpp
		Core/ECS/2D/Components/RigidBodyComponent.cpp
		Core/ECS/2D/Components/SpriteComponent.hpp
		Core/ECS/2D/Components/SpriteComponent.cpp
		Core/ECS/2D/Components/TransformComponent.hpp
		Core/ECS/2D/Components/TransformComponent.cpp
		Core/ECS/2D/Systems/RenderSystem.hpp
		Core/ECS/2D/Systems/RenderSystem.cpp
		Core/ECS/2D/Systems/PhysicsSystem.hpp
		Core/ECS/2D/Systems/PhysicsSystem.cpp
		Core/ECS/3D/Components/CameraComponent.hpp
		Core/ECS/3D/Components/CameraComponent.cpp
		Core/ECS/3D/Components/LightComponent.hpp
		Core/ECS/3D/Components/LightComponent.cpp
		Core/ECS/3D/Components/MeshComponent.hpp
		Core/ECS/3D/Components/MeshComponent.cpp
		Core/ECS/3D/Components/RigidBodyComponent.hpp
		Core/ECS/3D/Components/RigidBodyComponent.cpp
		Core/ECS/3D/Components/TransformComponent.hpp
		Core/ECS/3D/Components/TransformComponent.cpp
		Core/ECS/3D/Systems/RenderSystem.hpp
		Core/ECS/3D/Systems/RenderSystem.cpp
		Core/ECS/3D/Systems/PhysicsSystem.hpp
		Core/ECS/3D/Systems/PhysicsSystem.cpp
		Core/ECS/Audio/Components/AudioComponent.hpp
		Core/ECS/Audio/Components/AudioComponent.cpp
		Core/ECS/Audio/Systems/AudioSystem.hpp
		Core/ECS/Audio/Systems/AudioSystem.cpp
		Core/ECS/UI/Components/TextComponent.hpp
		Core/ECS/UI/Components/TextComponent.cpp
)

set(
	CoreExceptionFiles
		Core/Exception/CloveException.hpp
		Core/Exception/CloveException.cpp
)

set(
	CoreGraphicsFiles
		Core/Graphics/GraphicsTypes.hpp
		Core/Graphics/Material.hpp
		Core/Graphics/Material.inl
		Core/Graphics/Material.cpp
		Core/Graphics/MaterialInstance.hpp
		Core/Graphics/MaterialInstance.inl
		Core/Graphics/MaterialInstance.cpp
		Core/Graphics/PipelineObject.hpp
		Core/Graphics/RenderCommand.hpp
		Core/Graphics/RenderCommand.cpp
		Core/Graphics/RenderDevice.hpp
		Core/Graphics/RenderFactory.hpp
		Core/Graphics/RenderTarget.hpp
		Core/Graphics/Shader.hpp
		Core/Graphics/ShaderBufferTypes.hpp
		Core/Graphics/Surface.hpp
		Core/Graphics/VertexLayout.hpp
		Core/Graphics/VertexLayout.inl
		Core/Graphics/VertexLayout.cpp
		Core/Graphics/Renderables/Mesh.hpp
		Core/Graphics/Renderables/Mesh.cpp
		Core/Graphics/Renderables/Sprite.hpp
		Core/Graphics/Renderables/Sprite.cpp
		Core/Graphics/Resources/Buffer.hpp
		Core/Graphics/Resources/Texture.hpp
)

set(
	OpenGLGraphicsFiles
		Graphics/OpenGL/Resources/GLBuffer.hpp
		Graphics/OpenGL/Resources/GLBuffer.cpp
		Graphics/OpenGL/Resources/GLTexture.hpp
		Graphics/OpenGL/Resources/GLTexture.cpp
		Graphics/OpenGL/GL.hpp
		Graphics/OpenGL/GL.cpp
		Graphics/OpenGL/GLException.hpp
		Graphics/OpenGL/GLException.cpp
		Graphics/OpenGL/GLPipelineObject.hpp
		Graphics/OpenGL/GLPipelineObject.cpp
		Graphics/OpenGL/GLRenderDevice.hpp
		Graphics/OpenGL/GLRenderDevice.cpp
		Graphics/OpenGL/GLRenderFactory.hpp
		Graphics/OpenGL/GLRenderFactory.cpp
		Graphics/OpenGL/GLRenderTarget.hpp
		Graphics/OpenGL/GLRenderTarget.cpp
		Graphics/OpenGL/GLShader.hpp
		Graphics/OpenGL/GLShader.cpp
		Graphics/OpenGL/GLSurface.hpp
		Graphics/OpenGL/GLSurface.cpp
)

set(
	WindowsGLGraphicsFiles
		Graphics/OpenGL/WGLSurface.hpp
		Graphics/OpenGL/WGLSurface.cpp
)

set(
	LinuxGLGraphicsFiles
		Graphics/OpenGL/GLXSurface.hpp
		Graphics/OpenGL/GLXSurface.cpp
)

set(
	MacOSGLGraphicsFiles
		Graphics/OpenGL/CGLSurface.hpp
		Graphics/OpenGL/CGLSurface.mm
)

set(
	Direct3DGraphicsFiles
		Graphics/Direct3D/Resources/D3DBuffer.hpp
		Graphics/Direct3D/Resources/D3DBuffer.cpp
		Graphics/Direct3D/Resources/D3DTexture.hpp
		Graphics/Direct3D/Resources/D3DTexture.cpp
		Graphics/Direct3D/D3D.hpp
		Graphics/Direct3D/D3D.cpp
		Graphics/Direct3D/D3DException.hpp
		Graphics/Direct3D/D3DException.cpp
		Graphics/Direct3D/D3DPipelineObject.hpp
		Graphics/Direct3D/D3DPipelineObject.cpp
		Graphics/Direct3D/D3DRenderDevice.hpp
		Graphics/Direct3D/D3DRenderDevice.cpp
		Graphics/Direct3D/D3DRenderFactory.hpp
		Graphics/Direct3D/D3DRenderFactory.cpp
		Graphics/Direct3D/D3DRenderTarget.hpp
		Graphics/Direct3D/D3DRenderTarget.cpp
		Graphics/Direct3D/D3DShader.hpp
		Graphics/Direct3D/D3DShader.cpp
		Graphics/Direct3D/D3DSurface.hpp
		Graphics/Direct3D/D3DSurface.cpp
		Graphics/Direct3D/DXGIInfoManager.hpp
		Graphics/Direct3D/DXGIInfoManager.cpp
)

set(
	CoreInputFiles
		Core/Input/Input.hpp
		Core/Input/Input.cpp
		Core/Input/Keyboard.hpp
		Core/Input/Keyboard.inl
		Core/Input/Keyboard.cpp
		Core/Input/KeyCodes.hpp
		Core/Input/Mouse.hpp
		Core/Input/Mouse.cpp
		Core/Input/MouseButtonCodes.hpp
)

set(
	CoreMathsFiles
		Core/Maths/glmWrappers.hpp
		Core/Maths/Maths.hpp
		Core/Maths/Maths.inl
		Core/Maths/MathsHelpers.hpp
		Core/Maths/MathsHelpers.inl
		Core/Maths/MathsTypes.hpp
		Core/Maths/Matrix.hpp
		Core/Maths/Quaternion.hpp
		Core/Maths/Vector.hpp
)

set(
	CorePlatformFiles
		Core/Platform/Application.hpp
		Core/Platform/Application.cpp
		Core/Platform/PlatformTypes.hpp
		Core/Platform/Window.hpp
		Core/Platform/Window.cpp
)

set(
	WindowsPlatformFiles
		Platform/Windows/CloveWindows.hpp
		Platform/Windows/WindowsApplication.hpp
		Platform/Windows/WindowsApplication.cpp
		Platform/Windows/WindowsException.hpp
		Platform/Windows/WindowsException.cpp
		Platform/Windows/WindowsWindow.hpp
		Platform/Windows/WindowsWindow.cpp
)

set(
	LinuxPlatformFiles
		Platform/Linux/CloveLinux.hpp
		Platform/Linux/LinuxApplication.hpp
		Platform/Linux/LinuxApplication.cpp
		Platform/Linux/LinuxWindow.hpp
		Platform/Linux/LinuxWindow.cpp
)

set(
	MacOSPlatformFiles
		Platform/Mac/CloveMac.h
		Platform/Mac/MacApplication.hpp
		Platform/Mac/MacApplication.mm
		Platform/Mac/MacWindow.hpp
		Platform/Mac/MacWindow.mm
)

set(
	CoreProfilingFiles
		Core/Profiling/Timer.hpp
		Core/Profiling/Timer.cpp
)

set(
	CoreUIFiles
		Core/UI/Font.hpp
		Core/UI/Font.cpp
		Core/UI/Text.hpp
		Core/UI/Text.cpp
)

set(
	CoreUtilityFiles
		Core/Utils/Delegate.hpp
		Core/Utils/Delegate.inl
		Core/Utils/DeltaTime.hpp
		Core/Utils/DeltaTime.cpp
		Core/Utils/HashString.hpp
		Core/Utils/MeshLoader.hpp
		Core/Utils/MeshLoader.cpp
)

set(
	GLSLShaders
		Graphics/OpenGL/Shaders/2D-ps.glsl
        Graphics/OpenGL/Shaders/2D-vs.glsl
        Graphics/OpenGL/Shaders/CubeShadowMap-gs.glsl
		Graphics/OpenGL/Shaders/CubeShadowMap-ps.glsl
		Graphics/OpenGL/Shaders/CubeShadowMap-vs.glsl
		Graphics/OpenGL/Shaders/Font-ps.glsl
		Graphics/OpenGL/Shaders/Font-vs.glsl
		Graphics/OpenGL/Shaders/Lit-ps.glsl
		Graphics/OpenGL/Shaders/Lit-vs.glsl
		Graphics/OpenGL/Shaders/RT-ps.glsl
		Graphics/OpenGL/Shaders/RT-vs.glsl
		Graphics/OpenGL/Shaders/Unlit-ps.glsl
		Graphics/OpenGL/Shaders/Unlit-vs.glsl
)

#Parse the glsl shader files into a header 
set(GLSLParsedShaders)
set(GLSLShaderIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/OpenGL/ShaderStrings.hpp)
file(WRITE ${GLSLShaderIncludeFile} "//cmake generated header file\n\n")
foreach(shaderFile ${GLSLShaders})
	file(READ ${shaderFile} FILE_CONTENTS)
	get_filename_component(fileName ${shaderFile} NAME_WLE)

	string(REPLACE "-" "_" STRING_NAME ${fileName})
	set(outFilePath ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/OpenGL/Shaders/${fileName}.hpp)

	configure_file(../cmake/glslHeader.in ${outFilePath})

	set(GLSLParsedShaders ${GLSLParsedShaders} ${outFilePath})
	file(APPEND ${GLSLShaderIncludeFile} "#include \"Shaders/${fileName}.hpp\"\n")
endforeach()

set(
	OpenGLGraphicsFiles
		${OpenGLGraphicsFiles}
		${GLSLShaders}
		${GLSLShaderIncludeFile}
)

set(
	pixelShaders
        Graphics/Direct3D/Shaders/2D-ps.hlsl
        Graphics/Direct3D/Shaders/CubeShadowMap-ps.hlsl
		Graphics/Direct3D/Shaders/Font-ps.hlsl
		Graphics/Direct3D/Shaders/Lit-ps.hlsl
		Graphics/Direct3D/Shaders/RT-ps.hlsl
		Graphics/Direct3D/Shaders/Unlit-ps.hlsl
)
set(
	vertexShaders
        Graphics/Direct3D/Shaders/2D-vs.hlsl
        Graphics/Direct3D/Shaders/CubeShadowMap-vs.hlsl
		Graphics/Direct3D/Shaders/Font-vs.hlsl
		Graphics/Direct3D/Shaders/Lit-vs.hlsl
		Graphics/Direct3D/Shaders/RT-vs.hlsl
		Graphics/Direct3D/Shaders/Unlit-vs.hlsl
)
set(
    geometryShaders
	    Graphics/Direct3D/Shaders/CubeShadowMap-gs.hlsl
)

set(
	HLSLShaders
		${pixelShaders}
		${vertexShaders}
        ${geometryShaders}
)

set_property(SOURCE ${geometryShaders} PROPERTY VS_SHADER_TYPE Geometry)
set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_TYPE Vertex)
set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_TYPE Pixel)
set_property(SOURCE ${HLSLShaders} PROPERTY VS_SHADER_ENTRYPOINT main)
set_property(SOURCE ${HLSLShaders} PROPERTY VS_SHADER_MODEL 5.0)
#get each shader to compile into a header
set(HLSLShaderIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Direct3D/ShaderHeaders.hpp)
file(WRITE ${HLSLShaderIncludeFile} "//cmake generated header file\n\n")
foreach(shaderFile ${HLSLShaders})
	get_filename_component(fileName ${shaderFile} NAME_WLE)
	string(REPLACE "-" "_" STRING_NAME ${fileName})
	set(outFilePath ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/Direct3D/Shaders/${fileName}.hpp)
	set_property(SOURCE ${shaderFile} PROPERTY VS_SHADER_FLAGS "/Vn shader_${STRING_NAME} /Fh ${outFilePath}")
	file(APPEND ${HLSLShaderIncludeFile} "#include \"Shaders/${fileName}.hpp\"\n")
endforeach()

set(
	Direct3DGraphicsFiles
		${Direct3DGraphicsFiles}
		${HLSLShaders}
		${HLSLShaderIncludeFile}
)

set(
	AllFiles
	${CoreFiles}
		${CoreAudioFiles}
		${CoreECSFiles}
		${CoreExceptionFiles}
		${CoreGraphicsFiles}
			${OpenGLGraphicsFiles}
				${WindowsGLGraphicsFiles}
				${LinuxGLGraphicsFiles}
				${MacOSGLGraphicsFiles}
			${Direct3DGraphicsFiles}
		${CoreInputFiles}
		${CoreMathsFiles}
		${CorePlatformFiles}
			${WindowsPlatformFiles}
			${LinuxPlatformFiles}
			${MacOSPlatformFiles}
		${CoreProfilingFiles}
		${CoreUIFiles}
		${CoreUtilityFiles}
)

#Organise the source tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${AllFiles})

#Set the target source files
target_sources(
	${PROJECT_NAME}
	PRIVATE
		${AllFiles}
)

#Don't compile platform specific files
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set_source_files_properties(${LinuxGLGraphicsFiles} ${MacOSGLGraphicsFiles} ${LinuxPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_source_files_properties(${WindowsGLGraphicsFiles} ${MacOSGLGraphicsFiles} ${Direct3DGraphicsFiles} ${WindowsPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set_source_files_properties(${WindowsGLGraphicsFiles} ${LinuxGLGraphicsFiles} ${Direct3DGraphicsFiles} ${WindowsPlatformFiles} ${LinuxPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

#Includes
target_include_directories(
	${PROJECT_NAME}
	#Clove
	PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
	#Libraries
	PUBLIC
		../vendor/OBJ-Loader/Source
	INTERFACE
        ../vendor/glm
		#TODO: Get rid of the lbsndfile paths below (see Sound.hpp)
		../vendor/libsndfile/src
		${CMAKE_BINARY_DIR}/Clove/vendor/libsndfile/src
	PRIVATE
        ../vendor/OBJ-Loader/Source
		../vendor/freetype2/include
		../vendor/bullet3/src
		$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Clove/vendor/wglext>
)

#Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(CLV_COCOA_LIB Cocoa)
	find_library(CLV_MACOS_OPENGL_LIB OpenGL)
endif()

target_link_libraries(
	${PROJECT_NAME}
    #Cross platform
    PUBLIC
        spdlog
	PRIVATE 
		stb
		Glad
		glm
		sndfile
		portaudio_static
		freetype
		#Order of these are important
        BulletDynamics
        BulletCollision
        LinearMath
	#Windows
	PRIVATE
		$<$<PLATFORM_ID:Windows>:dxerr>
		$<$<PLATFORM_ID:Windows>:opengl32>
		$<$<PLATFORM_ID:Windows>:d3d11>
		$<$<PLATFORM_ID:Windows>:dxguid>
		$<$<PLATFORM_ID:Windows>:d3dcompiler>
	#Linux
	PRIVATE
		$<$<PLATFORM_ID:Linux>:GLX>
		$<$<PLATFORM_ID:Linux>:X11>
		$<$<PLATFORM_ID:Linux>:dl>
	#MacOS
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:${CLV_COCOA_LIB}>
		$<$<PLATFORM_ID:Darwin>:${CLV_MACOS_OPENGL_LIB}>
)

#Precompiled header
target_precompile_headers(
	${PROJECT_NAME}
	#Generic headers
	PUBLIC
		<iostream>
		<memory>
		<utility>
		<algorithm>
		<functional>
		<sstream>
		<optional>
	#Data structures
	PUBLIC
        <string>
        <array>
		<vector>
		<map>
		<unordered_map>
		<set>
		<unordered_set>
	#Clove headers
	PUBLIC
		[["Core/Core.hpp"]]
		[["Core/Log.hpp"]]
		[["Core/IntTypes.hpp"]]
		[["Core/Maths/MathsTypes.hpp"]]
		[["Core/Maths/Maths.hpp"]]
		[["Core/Maths/MathsHelpers.hpp"]]
		[["Core/Utils/Delegate.hpp"]]
	#Platform headers
	PUBLIC
		$<$<PLATFORM_ID:Windows>:Platform/Windows/CloveWindows.hpp>
		$<$<PLATFORM_ID:Linux>:Platform/Linux/CloveLinux.hpp>
)
source_group(Core FILES 
	${CMAKE_BINARY_DIR}/${PROJECT_NAME}/src/CmakeFiles/${PROJECT_NAME}.dir/cmake_pch.hxx 
	${CMAKE_BINARY_DIR}/${PROJECT_NAME}/src/CmakeFiles/${PROJECT_NAME}.dir/cmake_pch.cxx
)

#Preprocessor
target_compile_definitions(
	${PROJECT_NAME}
	#Config
	PUBLIC
		CLV_DEBUG=$<CONFIG:Debug>
		CLV_DEVELOPMENT=$<CONFIG:Development>
		CLV_PROFILING=$<CONFIG:Profiling>
		CLV_RELEASE=$<CONFIG:Release>
	#Platform
	PUBLIC
		CLV_PLATFORM_WINDOWS=$<PLATFORM_ID:Windows>
		CLV_PLATFORM_LINUX=$<PLATFORM_ID:Linux>
		CLV_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:GL_SILENCE_DEPRECATION> #Silences the MacOS gl deprecation messages
	#Engine
	INTERFACE
		CLV_ENGINE=0
	PRIVATE
		CLV_ENGINE=1
)