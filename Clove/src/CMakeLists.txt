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
	CoreExceptionFiles
		${CLV_INCLUDE}/Core/Exception/CloveException.hpp
		${CLV_SOURCE}/Core/Exception/CloveException.cpp
)

set(
	CoreGraphicsFiles
		${CLV_INCLUDE}/Graphics/Core/Resources/Buffer.hpp
		${CLV_INCLUDE}/Graphics/Core/Resources/Texture.hpp
		${CLV_INCLUDE}/Graphics/Core/CommandBuffer.hpp
		${CLV_INCLUDE}/Graphics/Core/Graphics.hpp		
		${CLV_SOURCE}/Graphics/Core/Graphics.cpp
		${CLV_INCLUDE}/Graphics/Core/GraphicsConstants.hpp
		${CLV_INCLUDE}/Graphics/Core/GraphicsTypes.hpp
		${CLV_INCLUDE}/Graphics/Core/PipelineObject.hpp
		${CLV_INCLUDE}/Graphics/Core/RenderFactory.hpp
		${CLV_INCLUDE}/Graphics/Core/RenderTarget.hpp
		${CLV_INCLUDE}/Graphics/Core/Shader.hpp
		${CLV_INCLUDE}/Graphics/Core/ShaderBufferTypes.hpp
		${CLV_INCLUDE}/Graphics/Core/Surface.hpp
		${CLV_INCLUDE}/Graphics/Core/VertexLayout.hpp
		${CLV_INCLUDE}/Graphics/Core/VertexLayout.inl
		${CLV_SOURCE}/Graphics/Core/VertexLayout.cpp	
)

set(
	Direct3DGraphicsFiles
		${CLV_INCLUDE}/Graphics/Direct3D/Resources/D3DBuffer.hpp
		${CLV_SOURCE}/Graphics/Direct3D/Resources/D3DBuffer.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/Resources/D3DTexture.hpp
		${CLV_SOURCE}/Graphics/Direct3D/Resources/D3DTexture.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3D.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3D.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DCommandBuffer.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DCommandBuffer.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DException.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DException.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DPipelineObject.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DPipelineObject.cpp
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
	MetalGraphicsFiles
		${CLV_INCLUDE}/Graphics/Metal/Resources/MTLBuffer.hpp
		${CLV_SOURCE}/Graphics/Metal/Resources/MTLBuffer.mm
		${CLV_INCLUDE}/Graphics/Metal/Resources/MTLTexture.hpp
		${CLV_SOURCE}/Graphics/Metal/Resources/MTLTexture.mm
		${CLV_INCLUDE}/Graphics/Metal/MTL.hpp
		${CLV_SOURCE}/Graphics/Metal/MTL.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLPipelineObject.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLPipelineObject.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLRenderDevice.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLRenderDevice.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLRenderFactory.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLRenderFactory.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLRenderTarget.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLRenderTarget.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLShader.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLShader.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLSurface.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLSurface.mm
)

set(
	OpenGLGraphicsFiles
		${CLV_INCLUDE}/Graphics/OpenGL/Resources/GLBuffer.hpp
		${CLV_SOURCE}/Graphics/OpenGL/Resources/GLBuffer.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/Resources/GLTexture.hpp
		${CLV_SOURCE}/Graphics/OpenGL/Resources/GLTexture.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GL.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GL.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLCommandBuffer.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLCommandBuffer.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLException.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLException.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLPipelineObject.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLPipelineObject.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLRenderFactory.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLRenderFactory.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLRenderTarget.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLRenderTarget.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLShader.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLShader.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLSurface.hpp
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
	CoreMemoryFiles
		${CLV_INCLUDE}/Memory/PoolAllocator.hpp
		${CLV_INCLUDE}/Memory/PoolAllocator.inl
)

set(
	CoreInputFiles
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
		${CLV_INCLUDE}/Platform/Core/Platform.hpp
		${CLV_SOURCE}/Platform/Core/Platform.cpp
		${CLV_INCLUDE}/Platform/Core/PlatformTypes.hpp
		${CLV_INCLUDE}/Platform/Core/Window.hpp
		${CLV_SOURCE}/Platform/Core/Window.cpp
)

set(
	WindowsPlatformFiles
		${CLV_INCLUDE}/Platform/Windows/CloveWindows.hpp
		${CLV_INCLUDE}/Platform/Windows/WindowsPlatform.hpp
		${CLV_SOURCE}/Platform/Windows/WindowsPlatform.cpp
		${CLV_INCLUDE}/Platform/Windows/WindowsException.hpp
		${CLV_SOURCE}/Platform/Windows/WindowsException.cpp
		${CLV_INCLUDE}/Platform/Windows/WindowsWindow.hpp
		${CLV_SOURCE}/Platform/Windows/WindowsWindow.cpp
)

set(
	LinuxPlatformFiles
		${CLV_INCLUDE}/Platform/Linux/CloveLinux.hpp
		${CLV_INCLUDE}/Platform/Linux/LinuxPlatform.hpp
		${CLV_SOURCE}/Platform/Linux/LinuxPlatform.cpp
		${CLV_INCLUDE}/Platform/Linux/LinuxWindow.hpp
		${CLV_SOURCE}/Platform/Linux/LinuxWindow.cpp
)

set(
	MacOSPlatformFiles
		${CLV_INCLUDE}/Platform/Mac/CloveMac.h
		${CLV_INCLUDE}/Platform/Mac/MacPlatform.hpp
		${CLV_SOURCE}/Platform/Mac/MacPlatform.mm
		${CLV_INCLUDE}/Platform/Mac/MacWindow.hpp
		${CLV_SOURCE}/Platform/Mac/MacWindow.mm
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
		${CLV_INCLUDE}/Core/Utils/Timer.hpp
		${CLV_SOURCE}/Core/Utils/Timer.cpp
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
		${CoreExceptionFiles}
		${CoreGraphicsFiles}
			${OpenGLGraphicsFiles}
				${WindowsGLGraphicsFiles}
				${LinuxGLGraphicsFiles}
			${Direct3DGraphicsFiles}
			${MetalGraphicsFiles}
		${CoreMemoryFiles}
		${CoreInputFiles}
		${CoreMathsFiles}
		${CorePlatformFiles}
			${WindowsPlatformFiles}
			${LinuxPlatformFiles}
			${MacOSPlatformFiles}
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