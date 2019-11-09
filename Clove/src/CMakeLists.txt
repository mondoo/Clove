#Clove CMake list

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files - Clove
set(
	CloveFiles
		Clove.hpp
		Core/Core.hpp
		Core/IntTypes.hpp
		Core/Layer.hpp
		Core/Layer.cpp
		Core/LayerStack.hpp
		Core/LayerStack.cpp
		Core/Log.hpp
		Core/Log.cpp
		Core/Audio/Sound.hpp
		Core/Audio/Sound.cpp
		Core/ECS/Component.hpp
		Core/ECS/Component.cpp
		Core/ECS/ECSTypes.hpp
		Core/ECS/Entity.hpp
		Core/ECS/Entity.inl
		Core/ECS/Entity.cpp
		Core/ECS/Manager.hpp
		Core/ECS/Manager.cpp
		Core/ECS/System.hpp
		Core/ECS/System.inl
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
		Core/ECS/3D/Systems/CameraSystem.hpp
		Core/ECS/3D/Systems/CameraSystem.cpp
		Core/ECS/3D/Systems/LightSystem.hpp
		Core/ECS/3D/Systems/LightSystem.cpp
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
		Core/ECS/UI/Systems/TextSystem.hpp
		Core/ECS/UI/Systems/TextSystem.cpp
		Core/Exception/CloveException.hpp
		Core/Exception/CloveException.cpp
		Core/Graphics/GraphicsTypes.hpp
		Core/Graphics/Material.hpp
		Core/Graphics/Material.inl
		Core/Graphics/Material.cpp
		Core/Graphics/MaterialInstance.hpp
		Core/Graphics/MaterialInstance.inl
		Core/Graphics/MaterialInstance.cpp
		Core/Graphics/Mesh.hpp
		Core/Graphics/Mesh.cpp
		Core/Graphics/RenderCommand.hpp
		Core/Graphics/RenderCommand.cpp
		Core/Graphics/RenderDevice.hpp
		Core/Graphics/Renderer.hpp
		Core/Graphics/Renderer.cpp
		Core/Graphics/Renderer2D.hpp
		Core/Graphics/Renderer2D.cpp
		Core/Graphics/RenderFactory.hpp
		Core/Graphics/RenderTarget.hpp
		Core/Graphics/RenderTarget.cpp
		Core/Graphics/Shader.hpp
		Core/Graphics/ShaderBufferTypes.hpp
		Core/Graphics/Sprite.hpp
		Core/Graphics/Sprite.cpp
		Core/Graphics/Surface.hpp
		Core/Graphics/VertexLayout.hpp
		Core/Graphics/VertexLayout.inl
		Core/Graphics/VertexLayout.cpp
		Core/Graphics/Resources/Buffer.hpp
		Core/Graphics/Resources/Texture.hpp
		Core/Input/Input.hpp
		Core/Input/Input.cpp
		Core/Input/Keyboard.hpp
		Core/Input/Keyboard.inl
		Core/Input/Keyboard.cpp
		Core/Input/KeyCodes.hpp
		Core/Input/Mouse.hpp
		Core/Input/Mouse.cpp
		Core/Input/MouseButtonCodes.hpp
		Core/Maths/glmWrappers.hpp
		Core/Maths/Maths.hpp
		Core/Maths/Maths.inl
		Core/Maths/MathsHelpers.hpp
		Core/Maths/MathsHelpers.inl
		Core/Maths/MathsTypes.hpp
		Core/Maths/Matrix.hpp
		Core/Maths/Quaternion.hpp
		Core/Maths/Vector.hpp
		Core/Platform/Application.hpp
		Core/Platform/Application.cpp
		Core/Platform/PlatformTypes.hpp
		Core/Platform/Window.hpp
		Core/Platform/Window.cpp
		Core/Profiling/Timer.hpp
		Core/Profiling/Timer.cpp
		Core/UI/Font.hpp
		Core/UI/Font.cpp
		Core/UI/Text.hpp
		Core/UI/Text.cpp
		Core/Utils/Delegate.hpp
		Core/Utils/Delegate.inl
		Core/Utils/DeltaTime.hpp
		Core/Utils/DeltaTime.cpp
		Core/Utils/HashString.hpp
		Core/Utils/MeshLoader.hpp
		Core/Utils/MeshLoader.cpp
)

#Files - Graphics | OpenGL
set(
	GraphicsFiles
		Graphics/OpenGL/Resources/GLBuffer.hpp
		Graphics/OpenGL/Resources/GLBuffer.cpp
		Graphics/OpenGL/Resources/GLTexture.hpp
		Graphics/OpenGL/Resources/GLTexture.cpp
		Graphics/OpenGL/GL.hpp
		Graphics/OpenGL/GL.cpp
		Graphics/OpenGL/GLRenderDevice.hpp
		Graphics/OpenGL/GLRenderDevice.cpp
		Graphics/OpenGL/GLRenderFactory.hpp
		Graphics/OpenGL/GLRenderFactory.cpp
		Graphics/OpenGL/GLShader.hpp
		Graphics/OpenGL/GLShader.cpp
		
		Graphics/OpenGL/GL4Exception.hpp
		Graphics/OpenGL/GL4Exception.cpp
		Graphics/OpenGL/GL4RenderTarget.hpp
		Graphics/OpenGL/GL4RenderTarget.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		GraphicsFiles
			${GraphicsFiles}	
			Graphics/OpenGL/WGLContext.hpp
			Graphics/OpenGL/WGLContext.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		GraphicsFiles
			${GraphicsFiles}
			Graphics/OpenGL/GLXContext.hpp
			Graphics/OpenGL/GLXContext.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(
		GraphicsFiles
			${GraphicsFiles}
			Graphics/OpenGL/CGLContext.hpp
			Graphics/OpenGL/CGLContext.mm
	)
endif()

#Files - Graphics | DirectX
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		GraphicsFiles
			${GraphicsFiles}
			Graphics/Direct3D/Resources/D3DBuffer.hpp
			Graphics/Direct3D/Resources/D3DBuffer.cpp
			Graphics/Direct3D/Resources/D3DTexture.hpp
			Graphics/Direct3D/Resources/D3DTexture.cpp
			Graphics/Direct3D/D3D.hpp
			Graphics/Direct3D/D3D.cpp
			Graphics/Direct3D/D3DException.hpp
			Graphics/Direct3D/D3DException.cpp
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
endif()

#Files - Graphics | Shaders
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
	GraphicsFiles
		${GraphicsFiles}
		${GLSLShaders}
		${GLSLShaderIncludeFile}
)

if(MSVC)
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
	GraphicsFiles
		${GraphicsFiles}
		${HLSLShaders}
		${HLSLShaderIncludeFile}
	)
endif()

#Files - Platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		PlatformFiles
			Platform/Windows/CloveWindows.hpp
			Platform/Windows/WindowsApplication.hpp
			Platform/Windows/WindowsApplication.cpp
			Platform/Windows/WindowsException.hpp
			Platform/Windows/WindowsException.cpp
			Platform/Windows/WindowsWindow.hpp
			Platform/Windows/WindowsWindow.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		PlatformFiles
			Platform/Linux/CloveLinux.hpp
			Platform/Linux/LinuxApplication.hpp
			Platform/Linux/LinuxApplication.cpp
			Platform/Linux/LinuxWindow.hpp
			Platform/Linux/LinuxWindow.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(
		PlatformFiles
			Platform/Mac/CloveMac.h
			Platform/Mac/MacApplication.hpp
			Platform/Mac/MacApplication.mm
			Platform/Mac/MacWindow.hpp
			Platform/Mac/MacWindow.mm
	)
endif()

#Organise the source tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CloveFiles})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GraphicsFiles})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PlatformFiles})

#Set the target source files
target_sources(
	${PROJECT_NAME}
	PRIVATE
		${CloveFiles}
		${GraphicsFiles}
		${PlatformFiles}
)

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
        ../vendor/Glad/include #would like to remove but the SBO forces it
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