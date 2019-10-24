#Clove CMake list

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files - Clove
set(
	CloveFiles
		Clove.hpp
		Clove/Core.hpp
		Clove/IntTypes.hpp
		Clove/Layer.hpp
		Clove/Layer.cpp
		Clove/LayerStack.hpp
		Clove/LayerStack.cpp
		Clove/Log.hpp
		Clove/Log.cpp
		Clove/Application.hpp
		Clove/Application.cpp
		Clove/Audio/Sound.hpp
		Clove/Audio/Sound.cpp
		Clove/ECS/Component.hpp
		Clove/ECS/Component.cpp
		Clove/ECS/ECSTypes.hpp
		Clove/ECS/Entity.hpp
		Clove/ECS/Entity.inl
		Clove/ECS/Entity.cpp
		Clove/ECS/Manager.hpp
		Clove/ECS/Manager.cpp
		Clove/ECS/System.hpp
		Clove/ECS/System.inl
		Clove/ECS/2D/Components/RenderableComponent.hpp
		Clove/ECS/2D/Components/RenderableComponent.cpp
		Clove/ECS/2D/Components/TransformComponent.hpp
		Clove/ECS/2D/Components/TransformComponent.cpp
		Clove/ECS/2D/Systems/RenderSystem.hpp
		Clove/ECS/2D/Systems/RenderSystem.cpp
		Clove/ECS/2D/Systems/TransformSystem.hpp
		Clove/ECS/2D/Systems/TransformSystem.cpp
		Clove/ECS/3D/Components/CameraComponent.hpp
		Clove/ECS/3D/Components/CameraComponent.cpp
		Clove/ECS/3D/Components/LightComponent.hpp
		Clove/ECS/3D/Components/LightComponent.cpp
		Clove/ECS/3D/Components/RenderableComponent.hpp
		Clove/ECS/3D/Components/RenderableComponent.cpp
		Clove/ECS/3D/Components/TransformComponent.hpp
		Clove/ECS/3D/Components/TransformComponent.cpp
		Clove/ECS/3D/Systems/CameraSystem.hpp
		Clove/ECS/3D/Systems/CameraSystem.cpp
		Clove/ECS/3D/Systems/LightSystem.hpp
		Clove/ECS/3D/Systems/LightSystem.cpp
		Clove/ECS/3D/Systems/RenderSystem.hpp
		Clove/ECS/3D/Systems/RenderSystem.cpp
		Clove/ECS/3D/Systems/TransformSystem.hpp
		Clove/ECS/3D/Systems/TransformSystem.cpp
		Clove/ECS/Audio/Components/AudioComponent.hpp
		Clove/ECS/Audio/Components/AudioComponent.cpp
		Clove/ECS/Audio/Systems/AudioSystem.hpp
		Clove/ECS/Audio/Systems/AudioSystem.cpp
		Clove/ECS/UI/Components/TextComponent.hpp
		Clove/ECS/UI/Components/TextComponent.cpp
		Clove/ECS/UI/Systems/TextSystem.hpp
		Clove/ECS/UI/Systems/TextSystem.cpp
		Clove/Exception/CloveException.hpp
		Clove/Exception/CloveException.cpp
		Clove/Graphics/Bindables/IndexBuffer.hpp
		Clove/Graphics/Bindables/IndexBuffer.cpp
		Clove/Graphics/Bindables/Shader.hpp
		Clove/Graphics/Bindables/Shader.cpp
		Clove/Graphics/Bindables/ShaderBufferObject.hpp
		Clove/Graphics/Bindables/Texture.hpp
		Clove/Graphics/Bindables/Texture.cpp
		Clove/Graphics/Bindables/VertexBuffer.hpp
		Clove/Graphics/Bindables/VertexBuffer.cpp
		Clove/Graphics/Bindable.hpp
		Clove/Graphics/Bindable.cpp
		Clove/Graphics/BindableFactory.hpp
		Clove/Graphics/BindableFactory.inl
		Clove/Graphics/BindableFactory.cpp
		Clove/Graphics/Context.hpp
		Clove/Graphics/Context.cpp
		Clove/Graphics/GraphicsTypes.hpp
		Clove/Graphics/Material.hpp
		Clove/Graphics/Material.inl
		Clove/Graphics/Material.cpp
		Clove/Graphics/MaterialInstance.hpp
		Clove/Graphics/MaterialInstance.inl
		Clove/Graphics/MaterialInstance.cpp
		Clove/Graphics/Mesh.hpp
		Clove/Graphics/Mesh.cpp
		Clove/Graphics/RenderAPI.hpp
		Clove/Graphics/RenderAPI.cpp
		Clove/Graphics/RenderCommand.hpp
		Clove/Graphics/RenderCommand.cpp
		Clove/Graphics/Renderer.hpp
		Clove/Graphics/Renderer.cpp
		Clove/Graphics/Renderer2D.hpp
		Clove/Graphics/Renderer2D.cpp
		Clove/Graphics/RenderTarget.hpp
		Clove/Graphics/RenderTarget.cpp
		Clove/Graphics/ShaderBufferTypes.hpp
		Clove/Graphics/Sprite.hpp
		Clove/Graphics/Sprite.cpp
		Clove/Graphics/VertexLayout.hpp
		Clove/Graphics/VertexLayout.inl
		Clove/Graphics/VertexLayout.cpp
		Clove/Input/Input.hpp
		Clove/Input/Input.cpp
		Clove/Input/Keyboard.hpp
		Clove/Input/Keyboard.inl
		Clove/Input/Keyboard.cpp
		Clove/Input/KeyCodes.hpp
		Clove/Input/Mouse.hpp
		Clove/Input/Mouse.cpp
		Clove/Input/MouseButtonCodes.hpp
		Clove/Maths/glmWrappers.hpp
		Clove/Maths/Maths.hpp
		Clove/Maths/Maths.inl
		Clove/Maths/MathsHelpers.hpp
		Clove/Maths/MathsHelpers.inl
		Clove/Maths/MathsTypes.hpp
		Clove/Maths/Matrix.hpp
		Clove/Maths/Quaternion.hpp
		Clove/Maths/Vector.hpp
		Clove/Platform/Window.hpp
		Clove/Platform/Window.cpp
		Clove/Profiling/Timer.hpp
		Clove/Profiling/Timer.cpp
		Clove/UI/Font.hpp
		Clove/UI/Font.cpp
		Clove/UI/Text.hpp
		Clove/UI/Text.cpp
		Clove/Utils/Delegate.hpp
		Clove/Utils/Delegate.inl
		Clove/Utils/DeltaTime.hpp
		Clove/Utils/DeltaTime.cpp
		Clove/Utils/MeshLoader.hpp
		Clove/Utils/MeshLoader.cpp
)

#Files - Graphics | OpenGL
set(
	GraphicsFiles
		Graphics/OpenGL-4/Bindables/GL4IndexBuffer.hpp
		Graphics/OpenGL-4/Bindables/GL4IndexBuffer.cpp
		Graphics/OpenGL-4/Bindables/GL4Shader.hpp
		Graphics/OpenGL-4/Bindables/GL4Shader.cpp
		Graphics/OpenGL-4/Bindables/GL4Texture.hpp
		Graphics/OpenGL-4/Bindables/GL4Texture.cpp
		Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.hpp
		Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.inl
		Graphics/OpenGL-4/Bindables/GL4VertexBuffer.hpp
		Graphics/OpenGL-4/Bindables/GL4VertexBuffer.cpp
		Graphics/OpenGL-4/GL4Exception.hpp
		Graphics/OpenGL-4/GL4Exception.cpp
		Graphics/OpenGL-4/GL4RenderAPI.hpp
		Graphics/OpenGL-4/GL4RenderAPI.cpp
		Graphics/OpenGL-4/GL4RenderTarget.hpp
		Graphics/OpenGL-4/GL4RenderTarget.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		GraphicsFiles
			${GraphicsFiles}	
			Graphics/OpenGL-4/WGLContext.hpp
			Graphics/OpenGL-4/WGLContext.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		GraphicsFiles
			${GraphicsFiles}
			Graphics/OpenGL-4/GLXContext.hpp
			Graphics/OpenGL-4/GLXContext.cpp
	)
endif()

#Files - Graphics | DirectX
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		GraphicsFiles
			${GraphicsFiles}	
			Graphics/DirectX-11/Bindables/DX11ConstantBuffer.hpp
			Graphics/DirectX-11/Bindables/DX11ConstantBuffer.inl
			Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp
			Graphics/DirectX-11/Bindables/DX11IndexBuffer.cpp
			Graphics/DirectX-11/Bindables/DX11Shader.hpp
			Graphics/DirectX-11/Bindables/DX11Shader.cpp
			Graphics/DirectX-11/Bindables/DX11Texture.hpp
			Graphics/DirectX-11/Bindables/DX11Texture.cpp
			Graphics/DirectX-11/Bindables/DX11VertexBuffer.hpp
			Graphics/DirectX-11/Bindables/DX11VertexBuffer.cpp
			Graphics/DirectX-11/Shaders/2D-ps.hlsl
			Graphics/DirectX-11/Shaders/2D-vs.hlsl
			Graphics/DirectX-11/Shaders/Font-ps.hlsl
			Graphics/DirectX-11/Shaders/Font-vs.hlsl
			Graphics/DirectX-11/Shaders/Lit-ps.hlsl
			Graphics/DirectX-11/Shaders/Lit-vs.hlsl
			Graphics/DirectX-11/Shaders/RT-ps.hlsl
			Graphics/DirectX-11/Shaders/RT-vs.hlsl
			Graphics/DirectX-11/Shaders/Unlit-ps.hlsl
			Graphics/DirectX-11/Shaders/Unlit-vs.hlsl
			Graphics/DirectX-11/DX11Exception.hpp
			Graphics/DirectX-11/DX11Exception.cpp
			Graphics/DirectX-11/DX11RenderAPI.hpp
			Graphics/DirectX-11/DX11RenderAPI.cpp
			Graphics/DirectX-11/DX11RenderTarget.hpp
			Graphics/DirectX-11/DX11RenderTarget.cpp
			Graphics/DirectX-11/DXGIInfoManager.hpp
			Graphics/DirectX-11/DXGIInfoManager.cpp
			
			Graphics/DirectX-11/DXContext.hpp
			Graphics/DirectX-11/DXContext.cpp
	)
endif()

#Files - Graphics | Shaders
set(
	GLSLShaders
		Graphics/OpenGL-4/Shaders/2D-ps.glsl
        Graphics/OpenGL-4/Shaders/2D-vs.glsl
        Graphics/OpenGL-4/Shaders/CubeShadowMap-gs.glsl
		Graphics/OpenGL-4/Shaders/CubeShadowMap-ps.glsl
		Graphics/OpenGL-4/Shaders/CubeShadowMap-vs.glsl
		Graphics/OpenGL-4/Shaders/Font-ps.glsl
		Graphics/OpenGL-4/Shaders/Font-vs.glsl
		Graphics/OpenGL-4/Shaders/Lit-ps.glsl
		Graphics/OpenGL-4/Shaders/Lit-vs.glsl
		Graphics/OpenGL-4/Shaders/RT-ps.glsl
		Graphics/OpenGL-4/Shaders/RT-vs.glsl
		Graphics/OpenGL-4/Shaders/Unlit-ps.glsl
		Graphics/OpenGL-4/Shaders/Unlit-vs.glsl
)

#Parse the glsl shader files into a header 
set(GLSLParsedShaders)
set(GLSLShaderIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/OpenGL-4/ShaderStrings.hpp)
file(WRITE ${GLSLShaderIncludeFile} "//cmake generated header file\n\n")
foreach(shaderFile ${GLSLShaders})
	file(READ ${shaderFile} FILE_CONTENTS)
	get_filename_component(fileName ${shaderFile} NAME_WLE)

	string(REPLACE "-" "_" STRING_NAME ${fileName})
	set(outFilePath ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/OpenGL-4/Shaders/${fileName}.hpp)

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
            Graphics/DirectX-11/Shaders/2D-ps.hlsl
            Graphics/DirectX-11/Shaders/CubeShadowMap-ps.hlsl
			Graphics/DirectX-11/Shaders/Font-ps.hlsl
			Graphics/DirectX-11/Shaders/Lit-ps.hlsl
			Graphics/DirectX-11/Shaders/RT-ps.hlsl
			Graphics/DirectX-11/Shaders/Unlit-ps.hlsl
	)

	set(
		vertexShaders
            Graphics/DirectX-11/Shaders/2D-vs.hlsl
            Graphics/DirectX-11/Shaders/CubeShadowMap-vs.hlsl
			Graphics/DirectX-11/Shaders/Font-vs.hlsl
			Graphics/DirectX-11/Shaders/Lit-vs.hlsl
			Graphics/DirectX-11/Shaders/RT-vs.hlsl
			Graphics/DirectX-11/Shaders/Unlit-vs.hlsl
	)

	set(
        geometryShaders
		    Graphics/DirectX-11/Shaders/CubeShadowMap-gs.hlsl
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
	set(HLSLShaderIncludeFile ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/DirectX-11/ShaderHeaders.hpp)
	file(WRITE ${HLSLShaderIncludeFile} "//cmake generated header file\n\n")
	foreach(shaderFile ${HLSLShaders})
		get_filename_component(fileName ${shaderFile} NAME_WLE)

		string(REPLACE "-" "_" STRING_NAME ${fileName})
		set(outFilePath ${CMAKE_CURRENT_SOURCE_DIR}/Graphics/DirectX-11/Shaders/${fileName}.hpp)

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
			Platform/Windows/WindowsWindow.hpp
			Platform/Windows/WindowsWindow.cpp
			Platform/Windows/WindowsException.hpp
			Platform/Windows/WindowsException.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		PlatformFiles
			Platform/Linux/CloveLinux.hpp
			Platform/Linux/LinuxWindow.hpp
			Platform/Linux/LinuxWindow.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(
		PlatformFiles
			Platform/Mac/CloveMac.h
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
	PRIVATE
		Graphics
		Platform	
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
		$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Clove/vendor/wglext>
)

#Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(COCOA_LIB Cocoa)
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
		$<$<PLATFORM_ID:Darwin>:${COCOA_LIB}>
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
		<unordered_map>
		<unordered_set>
	#Clove headers
	PUBLIC
		[["Clove/Core.hpp"]]
		[["Clove/Log.hpp"]]
		[["Clove/IntTypes.hpp"]]
		[["Clove/Maths/MathsTypes.hpp"]]
		[["Clove/Maths/Maths.hpp"]]
		[["Clove/Maths/MathsHelpers.hpp"]]
	#Platform headers
	PUBLIC
		$<$<PLATFORM_ID:Windows>:Platform/Windows/CloveWindows.hpp>
		$<$<PLATFORM_ID:Linux>:Platform/Linux/CloveLinux.hpp>
)
source_group(${PROJECT_NAME} FILES 
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
	#Engine
	INTERFACE
		CLV_ENGINE=0
	PRIVATE
		CLV_ENGINE=1
)