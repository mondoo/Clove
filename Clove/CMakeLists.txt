#Clove CMake list

project(Clove)

add_library(Clove STATIC)

#Files - Clove
set(
	CloveFiles
		src/Clove.hpp
		src/Clove/Core.hpp
		src/Clove/EntryPoint.hpp
		src/Clove/IntTypes.hpp
		src/Clove/Layer.hpp
		src/Clove/Layer.cpp
		src/Clove/LayerStack.hpp
		src/Clove/LayerStack.cpp
		src/Clove/Log.hpp
		src/Clove/Log.cpp
		src/Clove/Application.hpp
		src/Clove/Application.cpp
		src/Clove/Audio/Sound.hpp
		src/Clove/Audio/Sound.cpp
		src/Clove/ECS/Component.hpp
		src/Clove/ECS/Component.cpp
		src/Clove/ECS/ECSTypes.hpp
		src/Clove/ECS/Entity.hpp
		src/Clove/ECS/Entity.inl
		src/Clove/ECS/Entity.cpp
		src/Clove/ECS/Manager.hpp
		src/Clove/ECS/Manager.cpp
		src/Clove/ECS/System.hpp
		src/Clove/ECS/System.inl
		src/Clove/ECS/2D/Components/RenderableComponent.hpp
		src/Clove/ECS/2D/Components/RenderableComponent.cpp
		src/Clove/ECS/2D/Components/TransformComponent.hpp
		src/Clove/ECS/2D/Components/TransformComponent.cpp
		src/Clove/ECS/2D/Systems/RenderSystem.hpp
		src/Clove/ECS/2D/Systems/RenderSystem.cpp
		src/Clove/ECS/2D/Systems/TransformSystem.hpp
		src/Clove/ECS/2D/Systems/TransformSystem.cpp
		src/Clove/ECS/3D/Components/CameraComponent.hpp
		src/Clove/ECS/3D/Components/CameraComponent.cpp
		src/Clove/ECS/3D/Components/LightComponent.hpp
		src/Clove/ECS/3D/Components/LightComponent.cpp
		src/Clove/ECS/3D/Components/RenderableComponent.hpp
		src/Clove/ECS/3D/Components/RenderableComponent.cpp
		src/Clove/ECS/3D/Components/TransformComponent.hpp
		src/Clove/ECS/3D/Components/TransformComponent.cpp
		src/Clove/ECS/3D/Systems/CameraSystem.hpp
		src/Clove/ECS/3D/Systems/CameraSystem.cpp
		src/Clove/ECS/3D/Systems/LightSystem.hpp
		src/Clove/ECS/3D/Systems/LightSystem.cpp
		src/Clove/ECS/3D/Systems/RenderSystem.hpp
		src/Clove/ECS/3D/Systems/RenderSystem.cpp
		src/Clove/ECS/3D/Systems/TransformSystem.hpp
		src/Clove/ECS/3D/Systems/TransformSystem.cpp
		src/Clove/ECS/Audio/Components/AudioComponent.hpp
		src/Clove/ECS/Audio/Components/AudioComponent.cpp
		src/Clove/ECS/Audio/Systems/AudioSystem.hpp
		src/Clove/ECS/Audio/Systems/AudioSystem.cpp
		src/Clove/ECS/UI/Components/TextComponent.hpp
		src/Clove/ECS/UI/Components/TextComponent.cpp
		src/Clove/ECS/UI/Systems/TextSystem.hpp
		src/Clove/ECS/UI/Systems/TextSystem.cpp
		src/Clove/Exception/CloveException.hpp
		src/Clove/Exception/CloveException.cpp
		src/Clove/Graphics/Bindables/IndexBuffer.hpp
		src/Clove/Graphics/Bindables/IndexBuffer.cpp
		src/Clove/Graphics/Bindables/Shader.hpp
		src/Clove/Graphics/Bindables/Shader.cpp
		src/Clove/Graphics/Bindables/ShaderBufferObject.hpp
		src/Clove/Graphics/Bindables/Texture.hpp
		src/Clove/Graphics/Bindables/Texture.cpp
		src/Clove/Graphics/Bindables/VertexBuffer.hpp
		src/Clove/Graphics/Bindables/VertexBuffer.cpp
		src/Clove/Graphics/Bindable.hpp
		src/Clove/Graphics/Bindable.cpp
		src/Clove/Graphics/BindableFactory.hpp
		src/Clove/Graphics/BindableFactory.inl
		src/Clove/Graphics/BindableFactory.cpp
		src/Clove/Graphics/Context.hpp
		src/Clove/Graphics/Context.cpp
		src/Clove/Graphics/GraphicsTypes.hpp
		src/Clove/Graphics/Material.hpp
		src/Clove/Graphics/Material.inl
		src/Clove/Graphics/Material.cpp
		src/Clove/Graphics/MaterialInstance.hpp
		src/Clove/Graphics/MaterialInstance.inl
		src/Clove/Graphics/MaterialInstance.cpp
		src/Clove/Graphics/Mesh.hpp
		src/Clove/Graphics/Mesh.cpp
		src/Clove/Graphics/RenderAPI.hpp
		src/Clove/Graphics/RenderAPI.cpp
		src/Clove/Graphics/RenderCommand.hpp
		src/Clove/Graphics/RenderCommand.cpp
		src/Clove/Graphics/Renderer.hpp
		src/Clove/Graphics/Renderer.cpp
		src/Clove/Graphics/Renderer2D.hpp
		src/Clove/Graphics/Renderer2D.cpp
		src/Clove/Graphics/RenderTarget.hpp
		src/Clove/Graphics/RenderTarget.cpp
		src/Clove/Graphics/Sprite.hpp
		src/Clove/Graphics/Sprite.cpp
		src/Clove/Graphics/VertexLayout.hpp
		src/Clove/Graphics/VertexLayout.inl
		src/Clove/Graphics/VertexLayout.cpp
		src/Clove/Input/Input.hpp
		src/Clove/Input/Input.cpp
		src/Clove/Input/Keyboard.hpp
		src/Clove/Input/Keyboard.inl
		src/Clove/Input/Keyboard.cpp
		src/Clove/Input/KeyCodes.hpp
		src/Clove/Input/Mouse.hpp
		src/Clove/Input/Mouse.cpp
		src/Clove/Input/MouseButtonCodes.hpp
		src/Clove/Maths/glmWrappers.hpp
		src/Clove/Maths/Maths.hpp
		src/Clove/Maths/Maths.inl
		src/Clove/Maths/MathsHelpers.hpp
		src/Clove/Maths/MathsHelpers.inl
		src/Clove/Maths/MathsTypes.hpp
		src/Clove/Maths/Matrix.hpp
		src/Clove/Maths/Quaternion.hpp
		src/Clove/Maths/Vector.hpp
		src/Clove/Platform/Window.hpp
		src/Clove/Platform/Window.cpp
		src/Clove/Profiling/Timer.hpp
		src/Clove/Profiling/Timer.cpp
		src/Clove/UI/Font.hpp
		src/Clove/UI/Font.cpp
		src/Clove/UI/Text.hpp
		src/Clove/UI/Text.cpp
		src/Clove/Utils/Delegate.hpp
		src/Clove/Utils/Delegate.inl
		src/Clove/Utils/DeltaTime.hpp
		src/Clove/Utils/DeltaTime.cpp
		src/Clove/Utils/MeshLoader.hpp
		src/Clove/Utils/MeshLoader.cpp
)

#Files - Graphics
set(
	GraphicsFiles
		src/Graphics/OpenGL-4/Bindables/GL4IndexBuffer.hpp
		src/Graphics/OpenGL-4/Bindables/GL4IndexBuffer.cpp
		src/Graphics/OpenGL-4/Bindables/GL4Shader.hpp
		src/Graphics/OpenGL-4/Bindables/GL4Shader.cpp
		src/Graphics/OpenGL-4/Bindables/GL4Texture.hpp
		src/Graphics/OpenGL-4/Bindables/GL4Texture.cpp
		src/Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.hpp
		src/Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.inl
		src/Graphics/OpenGL-4/Bindables/GL4VertexBuffer.hpp
		src/Graphics/OpenGL-4/Bindables/GL4VertexBuffer.cpp
		src/Graphics/OpenGL-4/Shaders/2D-ps.glsl
		src/Graphics/OpenGL-4/Shaders/2D-vs.glsl
		src/Graphics/OpenGL-4/Shaders/Font-ps.glsl
		src/Graphics/OpenGL-4/Shaders/Font-vs.glsl
		src/Graphics/OpenGL-4/Shaders/Lit-ps.glsl
		src/Graphics/OpenGL-4/Shaders/Lit-vs.glsl
		src/Graphics/OpenGL-4/Shaders/RT-ps.glsl
		src/Graphics/OpenGL-4/Shaders/RT-vs.glsl
		src/Graphics/OpenGL-4/Shaders/Unlit-ps.glsl
		src/Graphics/OpenGL-4/Shaders/Unlit-vs.glsl
		src/Graphics/OpenGL-4/GL4Exception.hpp
		src/Graphics/OpenGL-4/GL4Exception.cpp
		src/Graphics/OpenGL-4/GL4RenderAPI.hpp
		src/Graphics/OpenGL-4/GL4RenderAPI.cpp
		src/Graphics/OpenGL-4/GL4RenderTarget.hpp
		src/Graphics/OpenGL-4/GL4RenderTarget.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		GraphicsFiles
			${GraphicsFiles}	

			src/Graphics/OpenGL-4/WGLContext.hpp
			src/Graphics/OpenGL-4/WGLContext.cpp

			src/Graphics/DirectX-11/Bindables/DX11ConstantBuffer.hpp
			src/Graphics/DirectX-11/Bindables/DX11ConstantBuffer.inl
			src/Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp
			src/Graphics/DirectX-11/Bindables/DX11IndexBuffer.cpp
			src/Graphics/DirectX-11/Bindables/DX11Shader.hpp
			src/Graphics/DirectX-11/Bindables/DX11Shader.cpp
			src/Graphics/DirectX-11/Bindables/DX11Texture.hpp
			src/Graphics/DirectX-11/Bindables/DX11Texture.cpp
			src/Graphics/DirectX-11/Bindables/DX11VertexBuffer.hpp
			src/Graphics/DirectX-11/Bindables/DX11VertexBuffer.cpp
			src/Graphics/DirectX-11/Shaders/2D-ps.hlsl
			src/Graphics/DirectX-11/Shaders/2D-vs.hlsl
			src/Graphics/DirectX-11/Shaders/Font-ps.hlsl
			src/Graphics/DirectX-11/Shaders/Font-vs.hlsl
			src/Graphics/DirectX-11/Shaders/Lit-ps.hlsl
			src/Graphics/DirectX-11/Shaders/Lit-vs.hlsl
			src/Graphics/DirectX-11/Shaders/RT-ps.hlsl
			src/Graphics/DirectX-11/Shaders/RT-vs.hlsl
			src/Graphics/DirectX-11/Shaders/Unlit-ps.hlsl
			src/Graphics/DirectX-11/Shaders/Unlit-vs.hlsl
			src/Graphics/DirectX-11/DX11Exception.hpp
			src/Graphics/DirectX-11/DX11Exception.cpp
			src/Graphics/DirectX-11/DX11RenderAPI.hpp
			src/Graphics/DirectX-11/DX11RenderAPI.cpp
			src/Graphics/DirectX-11/DX11RenderTarget.hpp
			src/Graphics/DirectX-11/DX11RenderTarget.cpp
			src/Graphics/DirectX-11/DXGIInfoManager.hpp
			src/Graphics/DirectX-11/DXGIInfoManager.cpp
			
			src/Graphics/DirectX-11/DXContext.hpp
			src/Graphics/DirectX-11/DXContext.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		GraphicsFiles
			${GraphicsFiles}

			src/Graphics/OpenGL-4/GLXContext.hpp
			src/Graphics/OpenGL-4/GLXContext.cpp
	)
endif()

#Files - Platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(
		PlatformFiles
			src/Platform/Windows/CloveWindows.hpp
			src/Platform/Windows/WindowsWindow.hpp
			src/Platform/Windows/WindowsWindow.cpp
			src/Platform/Windows/WindowsException.hpp
			src/Platform/Windows/WindowsException.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(
		PlatformFiles
			src/Platform/Linux/CloveLinux.hpp
			src/Platform/Linux/LinuxWindow.hpp
			src/Platform/Linux/LinuxWindow.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(
		PlatformFiles
			src/Platform/Mac/CloveMac.hpp
			src/Platform/Mac/MacWindow.h
			src/Platform/Mac/MacWindow.mm
	)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${CloveFiles})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${GraphicsFiles})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${PlatformFiles})

target_sources(
	Clove
	PRIVATE
		${CloveFiles}
		${GraphicsFiles}
		${PlatformFiles}
)

#Includes
target_include_directories(
	Clove

	#Clove
	PUBLIC
		src
	PRIVATE
		src/Graphics
		src/Platform	

	#Libraries
	PUBLIC
		vendor/OBJ-Loader/Source
	INTERFACE
		vendor/glm
		vendor/spdlog/include
		vendor/Glad/include #would like to remove but the SBO forces it
		#TODO: Get rid of the lbsndfile paths below (see Sound.hpp)
		vendor/libsndfile/src
		${CMAKE_BINARY_DIR}/Clove/vendor/libsndfile/src
	PRIVATE
		vendor/OBJ-Loader/Source
		vendor/freetype2/include
		$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Clove/vendor/wglext>
)

#temp - only find on mac?
find_library(COCOA_LIB Cocoa)

#Libraries
target_link_libraries(
	Clove
	PUBLIC
	PRIVATE 
		#Cross platform
		spdlog
		stb
		Glad
		glm
		sndfile
		portaudio_static
		freetype
		#Windows
		$<$<PLATFORM_ID:Windows>:dxerr>
		$<$<PLATFORM_ID:Windows>:opengl32>
		$<$<PLATFORM_ID:Windows>:d3d11>
		$<$<PLATFORM_ID:Windows>:dxguid>
		$<$<PLATFORM_ID:Windows>:d3dcompiler>
		#Linux
		$<$<PLATFORM_ID:Linux>:GLX>
		$<$<PLATFORM_ID:Linux>:X11>
		$<$<PLATFORM_ID:Linux>:dl>
		#Mac
		$<$<PLATFORM_ID:Darwin>:${COCOA_LIB}>
)

#Precompiled header
target_precompile_headers(
	${PROJECT_NAME}
	PUBLIC
		#Generic headers
		<iostream>
		<memory>
		<utility>
		<algorithm>
		<functional>
		<sstream>
		<optional>
		#Data structures
		<string>
		<vector>
		<unordered_map>
		<unordered_set>
		#Clove headers
		[["Clove/Core.hpp"]]
		[["Clove/Log.hpp"]]
		[["Clove/IntTypes.hpp"]]
		[["Clove/Maths/MathsTypes.hpp"]]
		[["Clove/Maths/Maths.hpp"]]
		[["Clove/Maths/MathsHelpers.hpp"]]
		#Platform headers
		$<$<PLATFORM_ID:Windows>:Platform/Windows/CloveWindows.hpp>
		$<$<PLATFORM_ID:Linux>:Platform/Linux/CloveLinux.hpp>
		$<$<PLATFORM_ID:Darwin>:Platform/Mac/CloveMac.hpp>
)
source_group(PCH FILES 
	${CMAKE_BINARY_DIR}/Clove/CmakeFiles/Clove.dir/cmake_pch.hxx 
	${CMAKE_BINARY_DIR}/Clove/CmakeFiles/Clove.dir/cmake_pch.cxx
)

#Preprocessor
target_compile_definitions(
	Clove
	PUBLIC
		CLV_PLATFORM_WINDOWS=$<PLATFORM_ID:Windows>
		CLV_PLATFORM_LINUX=$<PLATFORM_ID:Linux>
		CLV_PLATFORM_MACOS=$<PLATFORM_ID:Darwin>
)
target_compile_definitions(
	Clove
	INTERFACE
		CLV_ENGINE=0
	PRIVATE
		CLV_ENGINE=1
)

#DX Shaders
if(MSVC)
	set(pixelShaders
		src/Graphics/DirectX-11/Shaders/2D-ps.hlsl
		src/Graphics/DirectX-11/Shaders/Font-ps.hlsl
		src/Graphics/DirectX-11/Shaders/Lit-ps.hlsl
		src/Graphics/DirectX-11/Shaders/RT-ps.hlsl
		src/Graphics/DirectX-11/Shaders/Unlit-ps.hlsl
	)
	
	set(vertexShaders
		src/Graphics/DirectX-11/Shaders/2D-vs.hlsl
		src/Graphics/DirectX-11/Shaders/Font-vs.hlsl
		src/Graphics/DirectX-11/Shaders/Lit-vs.hlsl
		src/Graphics/DirectX-11/Shaders/RT-vs.hlsl
		src/Graphics/DirectX-11/Shaders/Unlit-vs.hlsl
	)

	set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_TYPE Pixel)
	set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_ENTRYPOINT main)
	set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_MODEL 5.0)

	set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_TYPE Vertex)
	set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_ENTRYPOINT main)
	set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_MODEL 5.0)

	set(shaderCSO
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/2D-ps.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/2D-vs.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Font-ps.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Font-vs.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Lit-ps.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Lit-vs.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/RT-ps.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/RT-vs.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Unlit-ps.cso
		 ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Unlit-vs.cso
	)

	add_custom_command(
		TARGET Clove POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${shaderCSO}
				${CMAKE_BINARY_DIR}/Sandbox/$<CONFIG>
	)
	add_custom_command(
		TARGET Clove POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${shaderCSO}
				${CMAKE_BINARY_DIR}/Sandbox
	)

endif()


#GL Shaders
set(GLShaders
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/2D-ps.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/2D-vs.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/Font-ps.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/Font-vs.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/Lit-ps.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/Lit-vs.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/RT-ps.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/RT-vs.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/Unlit-ps.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL-4/Shaders/Unlit-vs.glsl
)

#copy resource files - TODO: fix up sandbox references
if(MSVC)
	add_custom_command(
	        TARGET Clove POST_BUILD
	        COMMAND ${CMAKE_COMMAND} -E copy
	                ${GLShaders}
	                ${CMAKE_BINARY_DIR}/Sandbox/$<CONFIG>
	)
endif()
add_custom_command(
        TARGET Clove POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${GLShaders}
                ${CMAKE_BINARY_DIR}/Sandbox
)
add_custom_command(
        TARGET Clove POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Clove/res
                ${CMAKE_BINARY_DIR}/Sandbox/res
)

if(MSVC)
	add_custom_command(
	        TARGET Clove POST_BUILD
	        COMMAND ${CMAKE_COMMAND} -E copy_directory
	                ${CMAKE_SOURCE_DIR}/Sandbox/res
	                ${CMAKE_BINARY_DIR}/Sandbox/$<CONFIG>/res
	)
endif()
add_custom_command(
        TARGET Clove POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Sandbox/res
                ${CMAKE_BINARY_DIR}/Sandbox/res
)