set(CLV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(CLV_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Clove)

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files
set(
	CoreFiles
		${CLV_INCLUDE}/Clove.hpp
		${CLV_INCLUDE}/Core/Core.hpp
		${CLV_INCLUDE}/Core/IntTypes.hpp
		${CLV_INCLUDE}/Core/Layer.hpp
		${CLV_SOURCE}/Core/Layer.cpp
		${CLV_INCLUDE}/Core/LayerStack.hpp
		${CLV_SOURCE}/Core/LayerStack.cpp
		${CLV_INCLUDE}/Core/Log.hpp
		${CLV_SOURCE}/Core/Log.cpp
)

set(
	CoreAudioFiles
		${CLV_INCLUDE}/Core/Audio/Sound.hpp
		${CLV_SOURCE}/Core/Audio/Sound.cpp
)

set(
	CoreECSFiles
		${CLV_INCLUDE}/Core/ECS/Component.hpp
		${CLV_INCLUDE}/Core/ECS/Component.inl
		${CLV_INCLUDE}/Core/ECS/ComponentManager.hpp
		${CLV_INCLUDE}/Core/ECS/ComponentManager.inl
		${CLV_SOURCE}/Core/ECS/ComponentManager.cpp
		${CLV_INCLUDE}/Core/ECS/ECSTypes.hpp
		${CLV_INCLUDE}/Core/ECS/Entity.hpp
		${CLV_INCLUDE}/Core/ECS/Entity.inl
		${CLV_SOURCE}/Core/ECS/Entity.cpp
		${CLV_INCLUDE}/Core/ECS/Manager.hpp
		${CLV_INCLUDE}/Core/ECS/Manager.inl
		${CLV_SOURCE}/Core/ECS/Manager.cpp
		${CLV_INCLUDE}/Core/ECS/System.hpp
		${CLV_INCLUDE}/Core/ECS/2D/Components/RigidBodyComponent.hpp
		${CLV_SOURCE}/Core/ECS/2D/Components/RigidBodyComponent.cpp
		${CLV_INCLUDE}/Core/ECS/2D/Components/SpriteComponent.hpp
		${CLV_SOURCE}/Core/ECS/2D/Components/SpriteComponent.cpp
		${CLV_INCLUDE}/Core/ECS/2D/Components/TransformComponent.hpp
		${CLV_SOURCE}/Core/ECS/2D/Components/TransformComponent.cpp
		${CLV_INCLUDE}/Core/ECS/2D/Systems/RenderSystem.hpp
		${CLV_SOURCE}/Core/ECS/2D/Systems/RenderSystem.cpp
		${CLV_INCLUDE}/Core/ECS/2D/Systems/PhysicsSystem.hpp
		${CLV_SOURCE}/Core/ECS/2D/Systems/PhysicsSystem.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Components/CameraComponent.hpp
		${CLV_SOURCE}/Core/ECS/3D/Components/CameraComponent.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Components/LightComponent.hpp
		${CLV_SOURCE}/Core/ECS/3D/Components/LightComponent.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Components/MeshComponent.hpp
		${CLV_SOURCE}/Core/ECS/3D/Components/MeshComponent.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Components/RigidBodyComponent.hpp
		${CLV_SOURCE}/Core/ECS/3D/Components/RigidBodyComponent.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Components/TransformComponent.hpp
		${CLV_SOURCE}/Core/ECS/3D/Components/TransformComponent.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Systems/RenderSystem.hpp
		${CLV_SOURCE}/Core/ECS/3D/Systems/RenderSystem.cpp
		${CLV_INCLUDE}/Core/ECS/3D/Systems/PhysicsSystem.hpp
		${CLV_SOURCE}/Core/ECS/3D/Systems/PhysicsSystem.cpp
		${CLV_INCLUDE}/Core/ECS/Audio/Components/AudioComponent.hpp
		${CLV_SOURCE}/Core/ECS/Audio/Components/AudioComponent.cpp
		${CLV_INCLUDE}/Core/ECS/Audio/Systems/AudioSystem.hpp
		${CLV_SOURCE}/Core/ECS/Audio/Systems/AudioSystem.cpp
		${CLV_INCLUDE}/Core/ECS/UI/Components/TextComponent.hpp
		${CLV_SOURCE}/Core/ECS/UI/Components/TextComponent.cpp
		${CLV_INCLUDE}/Core/ECS/UI/Components/TransformComponent.hpp
		${CLV_SOURCE}/Core/ECS/UI/Components/TransformComponent.cpp
		${CLV_INCLUDE}/Core/ECS/UI/Components/WidgetComponent.hpp
		${CLV_SOURCE}/Core/ECS/UI/Components/WidgetComponent.cpp
)

set(
	CoreExceptionFiles
		${CLV_INCLUDE}/Core/Exception/CloveException.hpp
		${CLV_SOURCE}/Core/Exception/CloveException.cpp
)

set(
	CoreGraphicsFiles
		${CLV_INCLUDE}/Core/Graphics/GraphicsTypes.hpp
		${CLV_INCLUDE}/Core/Graphics/Material.hpp
		${CLV_INCLUDE}/Core/Graphics/Material.inl
		${CLV_SOURCE}/Core/Graphics/Material.cpp
		${CLV_INCLUDE}/Core/Graphics/MaterialInstance.hpp
		${CLV_INCLUDE}/Core/Graphics/MaterialInstance.inl
		${CLV_SOURCE}/Core/Graphics/MaterialInstance.cpp
		${CLV_INCLUDE}/Core/Graphics/PipelineObject.hpp
		${CLV_INCLUDE}/Core/Graphics/RenderCommand.hpp
		${CLV_SOURCE}/Core/Graphics/RenderCommand.cpp
		${CLV_INCLUDE}/Core/Graphics/RenderDevice.hpp
		${CLV_INCLUDE}/Core/Graphics/RenderFactory.hpp
		${CLV_INCLUDE}/Core/Graphics/RenderTarget.hpp
		${CLV_INCLUDE}/Core/Graphics/Shader.hpp
		${CLV_INCLUDE}/Core/Graphics/ShaderBufferTypes.hpp
		${CLV_INCLUDE}/Core/Graphics/Surface.hpp
		${CLV_INCLUDE}/Core/Graphics/VertexLayout.hpp
		${CLV_INCLUDE}/Core/Graphics/VertexLayout.inl
		${CLV_SOURCE}/Core/Graphics/VertexLayout.cpp
		${CLV_INCLUDE}/Core/Graphics/Renderables/Mesh.hpp
		${CLV_SOURCE}/Core/Graphics/Renderables/Mesh.cpp
		${CLV_INCLUDE}/Core/Graphics/Renderables/Sprite.hpp
		${CLV_SOURCE}/Core/Graphics/Renderables/Sprite.cpp
		${CLV_INCLUDE}/Core/Graphics/Resources/Buffer.hpp
		${CLV_INCLUDE}/Core/Graphics/Resources/Texture.hpp
)

set(
	OpenGLGraphicsFiles
		${CLV_INCLUDE}/Graphics/OpenGL/Resources/GLBuffer.hpp
		${CLV_SOURCE}/Graphics/OpenGL/Resources/GLBuffer.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/Resources/GLTexture.hpp
		${CLV_SOURCE}/Graphics/OpenGL/Resources/GLTexture.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GL.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GL.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLException.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLException.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLPipelineObject.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLPipelineObject.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLRenderDevice.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLRenderDevice.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLRenderFactory.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLRenderFactory.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLRenderTarget.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLRenderTarget.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLShader.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLShader.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLSurface.hpp
)

set(
	MetalGraphicsFiles
		${CLV_INCLUDE}/Graphics/Metal/Resources/MTLBuffer.hpp
		${CLV_SOURCE}/Graphics/Metal/Resources/MTLBuffer.mm
		${CLV_INCLUDE}/Graphics/Metal/Resources/MTLTexture.hpp
		${CLV_SOURCE}/Graphics/Metal/Resources/MTLTexture.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLShader.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLShader.mm
)

set(
	WindowsGLGraphicsFiles
		${CLV_INCLUDE}/Graphics/OpenGL/WGLSurface.hpp
		${CLV_SOURCE}/Graphics/OpenGL/WGLSurface.cpp
)

set(
	LinuxGLGraphicsFiles
		${CLV_INCLUDE}/Graphics/OpenGL/GLXSurface.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLXSurface.cpp
)

set(
	Direct3DGraphicsFiles
		${CLV_INCLUDE}/Graphics/Direct3D/Resources/D3DBuffer.hpp
		${CLV_SOURCE}/Graphics/Direct3D/Resources/D3DBuffer.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/Resources/D3DTexture.hpp
		${CLV_SOURCE}/Graphics/Direct3D/Resources/D3DTexture.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3D.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3D.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DException.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DException.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DPipelineObject.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DPipelineObject.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DRenderDevice.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DRenderDevice.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DRenderFactory.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DRenderFactory.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DRenderTarget.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DRenderTarget.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DShader.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DShader.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DSurface.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DSurface.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/DXGIInfoManager.hpp
		${CLV_SOURCE}/Graphics/Direct3D/DXGIInfoManager.cpp
)

set(
	CoreInputFiles
		${CLV_INCLUDE}/Core/Input/Input.hpp
		${CLV_SOURCE}/Core/Input/Input.cpp
		${CLV_INCLUDE}/Core/Input/Keyboard.hpp
		${CLV_INCLUDE}/Core/Input/Keyboard.inl
		${CLV_SOURCE}/Core/Input/Keyboard.cpp
		${CLV_INCLUDE}/Core/Input/KeyCodes.hpp
		${CLV_INCLUDE}/Core/Input/Mouse.hpp
		${CLV_SOURCE}/Core/Input/Mouse.cpp
		${CLV_INCLUDE}/Core/Input/MouseButtonCodes.hpp
)

set(
	CoreMathsFiles
		${CLV_INCLUDE}/Core/Maths/glmWrappers.hpp
		${CLV_INCLUDE}/Core/Maths/Maths.hpp
		${CLV_INCLUDE}/Core/Maths/Maths.inl
		${CLV_INCLUDE}/Core/Maths/MathsHelpers.hpp
		${CLV_INCLUDE}/Core/Maths/MathsHelpers.inl
		${CLV_INCLUDE}/Core/Maths/MathsTypes.hpp
		${CLV_INCLUDE}/Core/Maths/Matrix.hpp
		${CLV_INCLUDE}/Core/Maths/Quaternion.hpp
		${CLV_INCLUDE}/Core/Maths/Vector.hpp
)

set(
	CorePlatformFiles
		${CLV_INCLUDE}/Core/Platform/Application.hpp
		${CLV_SOURCE}/Core/Platform/Application.cpp
		${CLV_INCLUDE}/Core/Platform/PlatformTypes.hpp
		${CLV_INCLUDE}/Core/Platform/Window.hpp
		${CLV_SOURCE}/Core/Platform/Window.cpp
)

set(
	WindowsPlatformFiles
		${CLV_INCLUDE}/Platform/Windows/CloveWindows.hpp
		${CLV_INCLUDE}/Platform/Windows/WindowsApplication.hpp
		${CLV_SOURCE}/Platform/Windows/WindowsApplication.cpp
		${CLV_INCLUDE}/Platform/Windows/WindowsException.hpp
		${CLV_SOURCE}/Platform/Windows/WindowsException.cpp
		${CLV_INCLUDE}/Platform/Windows/WindowsWindow.hpp
		${CLV_SOURCE}/Platform/Windows/WindowsWindow.cpp
)

set(
	LinuxPlatformFiles
		${CLV_INCLUDE}/Platform/Linux/CloveLinux.hpp
		${CLV_INCLUDE}/Platform/Linux/LinuxApplication.hpp
		${CLV_SOURCE}/Platform/Linux/LinuxApplication.cpp
		${CLV_INCLUDE}/Platform/Linux/LinuxWindow.hpp
		${CLV_SOURCE}/Platform/Linux/LinuxWindow.cpp
)

set(
	MacOSPlatformFiles
		${CLV_INCLUDE}/Platform/Mac/CloveMac.h
		${CLV_INCLUDE}/Platform/Mac/MacApplication.hpp
		${CLV_SOURCE}/Platform/Mac/MacApplication.mm
		${CLV_INCLUDE}/Platform/Mac/MacWindow.hpp
		${CLV_SOURCE}/Platform/Mac/MacWindow.mm
)

set(
	CoreProfilingFiles
		${CLV_INCLUDE}/Core/Profiling/Timer.hpp
		${CLV_SOURCE}/Core/Profiling/Timer.cpp
)

set(
	CoreUIFiles
		${CLV_INCLUDE}/Core/UI/Font.hpp
		${CLV_SOURCE}/Core/UI/Font.cpp
		${CLV_INCLUDE}/Core/UI/Text.hpp
		${CLV_SOURCE}/Core/UI/Text.cpp
)

set(
	CoreUtilityFiles
		${CLV_INCLUDE}/Core/Utils/Delegate.hpp
		${CLV_INCLUDE}/Core/Utils/Delegate.inl
		${CLV_INCLUDE}/Core/Utils/DeltaTime.hpp
		${CLV_SOURCE}/Core/Utils/DeltaTime.cpp
		${CLV_INCLUDE}/Core/Utils/HashString.hpp
		${CLV_INCLUDE}/Core/Utils/MeshLoader.hpp
		${CLV_SOURCE}/Core/Utils/MeshLoader.cpp
)

set(
	GLSLShaders
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/2D-ps.glsl
        ${CLV_SOURCE}/Graphics/OpenGL/Shaders/2D-vs.glsl
        ${CLV_SOURCE}/Graphics/OpenGL/Shaders/CubeShadowMap-gs.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/CubeShadowMap-ps.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/CubeShadowMap-vs.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/Font-ps.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/Font-vs.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/Lit-ps.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/Lit-vs.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/RT-ps.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/RT-vs.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/Unlit-ps.glsl
		${CLV_SOURCE}/Graphics/OpenGL/Shaders/Unlit-vs.glsl
)

#Parse the glsl shader files into a header
set(GLSLShaderIncludeFile ${CLV_INCLUDE}/Graphics/OpenGL/ShaderStrings.hpp)
file(WRITE ${GLSLShaderIncludeFile} "//cmake generated header file\n\n")
foreach(shaderFile ${GLSLShaders})
	file(READ ${shaderFile} FILE_CONTENTS)
	get_filename_component(fileName ${shaderFile} NAME_WLE)

	string(REPLACE "-" "_" STRING_NAME ${fileName})
	set(outFilePath ${CLV_INCLUDE}/Graphics/OpenGL/Shaders/${fileName}.hpp)

	configure_file(../cmake/glslHeader.in ${outFilePath})

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
        ${CLV_SOURCE}/Graphics/Direct3D/Shaders/2D-ps.hlsl
        ${CLV_SOURCE}/Graphics/Direct3D/Shaders/CubeShadowMap-ps.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/Font-ps.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/Lit-ps.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/RT-ps.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/Unlit-ps.hlsl
)
set(
	vertexShaders
        ${CLV_SOURCE}/Graphics/Direct3D/Shaders/2D-vs.hlsl
        ${CLV_SOURCE}/Graphics/Direct3D/Shaders/CubeShadowMap-vs.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/Font-vs.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/Lit-vs.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/RT-vs.hlsl
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/Unlit-vs.hlsl
)
set(
    geometryShaders
		${CLV_SOURCE}/Graphics/Direct3D/Shaders/CubeShadowMap-gs.hlsl
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
set(HLSLShaderIncludeFile ${CLV_INCLUDE}/Graphics/Direct3D/ShaderHeaders.hpp)
file(WRITE ${HLSLShaderIncludeFile} "//cmake generated header file\n\n")
foreach(shaderFile ${HLSLShaders})
	get_filename_component(fileName ${shaderFile} NAME_WLE)
	string(REPLACE "-" "_" STRING_NAME ${fileName})
	set(outFilePath ${CLV_INCLUDE}/Graphics/Direct3D/Shaders/${fileName}.hpp)
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
	MSLShaders
		${CLV_SOURCE}/Graphics/Metal/Shaders/2D.metal
		${CLV_SOURCE}/Graphics/Metal/Shaders/Font.metal
		${CLV_SOURCE}/Graphics/Metal/Shaders/Unlit.metal
)

set_source_files_properties(${MetalShaders} PROPERTIES LANGUAGE METAL)

#Parse each metal shader file into a header
set(MSLShaderIncudeFile ${CLV_INCLUDE}/Graphics/Metal/ShaderStrings.hpp)
file(WRITE ${MSLShaderIncudeFile} "//cmake generated header file\n\n")
foreach(shaderFile ${MSLShaders})
	file(READ ${shaderFile} FILE_CONTENTS)
	get_filename_component(fileName ${shaderFile} NAME_WLE)

	set(STRING_NAME ${fileName})
	set(outFilePath ${CLV_INCLUDE}/Graphics/Metal/Shaders/${fileName}.hpp)

	configure_file(../cmake/glslHeader.in ${outFilePath})

	file(APPEND ${MSLShaderIncudeFile} "#include \"Shaders/${fileName}.hpp\"\n")
endforeach()

set(
	MetalGraphicsFiles
		${MetalGraphicsFiles}
		${MSLShaders}
		${MSLShaderIncudeFile}
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
			${Direct3DGraphicsFiles}
			${MetalGraphicsFiles}
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
file(GLOB_RECURSE header ${CLV_INCLUDE}/*.h ${CLV_INCLUDE}/*.hpp ${CLV_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm *.glsl *.hlsl *.metal)
source_group(TREE ${CLV_INCLUDE} FILES ${header})
source_group(TREE ${CLV_SOURCE} FILES ${source})

source_group(Core FILES 
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/${PROJECT_NAME}.dir/cmake_pch.hxx 
	${CMAKE_CURRENT_BINARY_DIR}/CmakeFiles/${PROJECT_NAME}.dir/cmake_pch.cxx
)

#Set the target source files
target_sources(
	${PROJECT_NAME}
	PRIVATE
		${AllFiles}
)

#Don't compile platform specific files
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set_source_files_properties(${LinuxGLGraphicsFiles} ${MetalGraphicsFiles} ${LinuxPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_source_files_properties(${WindowsGLGraphicsFiles} ${Direct3DGraphicsFiles} ${MetalGraphicsFiles} ${WindowsPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set_source_files_properties(${WindowsGLGraphicsFiles} ${LinuxGLGraphicsFiles} ${Direct3DGraphicsFiles} ${WindowsPlatformFiles} ${LinuxPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()