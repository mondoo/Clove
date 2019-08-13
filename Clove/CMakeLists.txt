#Clove CMake list

project(Clove)

add_library(Clove STATIC)

#Files - Clove
target_sources(
	Clove
	PUBLIC
		src/clvpch.hpp
		src/clvpch.cpp
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
		src/Clove/ECS/Component.hpp
		src/Clove/ECS/Component.cpp
		src/Clove/ECS/ECSTypes.hpp
		src/Clove/ECS/Entity.hpp
		src/Clove/ECS/Entity.inl
		src/Clove/ECS/Entity.cpp
		src/Clove/ECS/Manager.hpp
		src/Clove/ECS/Manager.inl
		src/Clove/ECS/Manager.cpp
		src/Clove/ECS/System.hpp
		src/Clove/ECS/System.inl
		src/Clove/ECS/2D/Components/RenderableComponent.hpp
		src/Clove/ECS/2D/Components/RenderableComponent.cpp
		src/Clove/ECS/2D/Components/TransformComponent.hpp
		src/Clove/ECS/2D/Components/TransformComponent.cpp
		src/Clove/ECS/2D/Systems/RenderSystem.hpp
		src/Clove/ECS/2D/Systems/RenderSystem.cpp
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
		src/Clove/Graphics/RenderAPI.hpp
		src/Clove/Graphics/RenderAPI.cpp
		src/Clove/Graphics/RenderCommand.hpp
		src/Clove/Graphics/RenderCommand.cpp
		src/Clove/Graphics/Renderer.hpp
		src/Clove/Graphics/Renderer.cpp
		src/Clove/Graphics/RenderTarget.hpp
		src/Clove/Graphics/RenderTarget.cpp
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
		src/Clove/Profiling/Timer.hpp
		src/Clove/Utils/Delegate.hpp
		src/Clove/Utils/Delegate.inl
		src/Clove/Utils/DeltaTime.hpp
		src/Clove/Utils/DeltaTime.cpp
		src/Clove/Utils/MeshLoader.hpp
		src/Clove/Utils/MeshLoader.cpp
)

#Files - Graphics
target_sources(
	Clove
	PRIVATE
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
	target_sources(
		Clove
		PRIVATE
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
			src/Graphics/DirectX-11/DXContext.hpp
			src/Graphics/DirectX-11/DXContext.cpp
			src/Graphics/DirectX-11/DXGIInfoManager.hpp
			src/Graphics/DirectX-11/DXGIInfoManager.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_sources(
		Clove
		PRIVATE
			src/Graphics/OpenGL-4/GLXContext.hpp
			src/Graphics/OpenGL-4/GLXContext.cpp
	)
endif()

#Files - Platform
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_sources(
		Clove
		PRIVATE
			src/Platform/Linux/LinuxWindow.hpp
			src/Platform/Linux/LinuxWindow.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_sources(
		Clove
		PRIVATE
			src/Platform/Windows/WindowsWindow.hpp
			src/Platform/Windows/WindowsWindow.cpp
	)
endif()

#Includes
target_include_directories(
	Clove

	#Clove
	PUBLIC
		src		#does this null out the two privates below?
		#src/Clove
	PRIVATE
		src/Graphics
		src/Platform	

	#Libraries
	PUBLIC
		vendor/glm
		vendor/OBJ-Loader/Source
	INTERFACE
		vendor/spdlog/include
		vendor/Glad/include #would like to remove but the SBO forces it
	PRIVATE
		vendor/dxerr
		#vendor/Glad/include
		#vendor/wglext
		#vendor/stb
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
target_include_directories(
		Clove
		PUBLIC
			vendor/wglext
	)
endif()

#Libraries
target_link_libraries(
	Clove
	PUBLIC
		stb
	PRIVATE 
		dxerr #TODO: Only add on windows?
		Glad
		glm
		spdlog
)

##How to get everywhere?
#if(CMAKE_CONFIGURATION_TYPES)
#  set(CMAKE_CONFIGURATION_TYPES Debug Development Profiling Release)
#  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
#    "Reset the configurations to what we need"
#    FORCE)
#endif()

#Precompiled header
if(MSVC)
	set_target_properties(Clove PROPERTIES COMPILE_FLAGS "/Yuclvpch.hpp")
	set_source_files_properties(src/clvpch.cpp PROPERTIES COMPILE_FLAGS "/Ycclvpch.hpp" OBJECT_OUTPUTS "$(IntDir)/clvpch.pch")
endif()

#Preprocessor
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(
		Clove
		PUBLIC
			CLV_PLATFORM_WINDOWS=1
			CLV_PLATFORM_LINUX=0
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(
		Clove
		PUBLIC
			CLV_PLATFORM_WINDOWS=0
			CLV_PLATFORM_LINUX=1
	)
endif()

target_compile_definitions(
	Clove
	PUBLIC
		CLV_DEVELOPMENT=1
	INTERFACE
		CLV_ENGINE=0
	PRIVATE
		CLV_ENGINE=1
)

#Shaders
if(MSVC)
	set(pixelShaders
		src/Graphics/DirectX-11/Shaders/2D-ps.hlsl
		src/Graphics/DirectX-11/Shaders/Lit-ps.hlsl
		src/Graphics/DirectX-11/Shaders/RT-ps.hlsl
		src/Graphics/DirectX-11/Shaders/Unlit-ps.hlsl
	)
	
	set(vertexShaders
		src/Graphics/DirectX-11/Shaders/2D-vs.hlsl
		src/Graphics/DirectX-11/Shaders/Lit-vs.hlsl
		src/Graphics/DirectX-11/Shaders/RT-vs.hlsl
		src/Graphics/DirectX-11/Shaders/Unlit-vs.hlsl
	)

	set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_TYPE Pixel)
	set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_ENTRYPOINT main)
	set_property(SOURCE ${pixelShaders} PROPERTY VS_SHADER_MODEL 6.3)

	set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_TYPE Vertex)
	set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_ENTRYPOINT main)
	set_property(SOURCE ${vertexShaders} PROPERTY VS_SHADER_MODEL 6.3)
endif()

message(CMAKE_WINDOWS_KITS_10_DIR)