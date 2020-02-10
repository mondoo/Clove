set(CLV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(CLV_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Clove)

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files
set(
	CoreFiles
		${CLV_INCLUDE}/Clove.hpp
		${CLV_INCLUDE}/Core/Core.hpp
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
		${CLV_INCLUDE}/Graphics/Core/GraphicsTypes.hpp
		${CLV_INCLUDE}/Graphics/Core/PipelineObject.hpp
		${CLV_INCLUDE}/Graphics/Core/GraphicsFactory.hpp
		${CLV_INCLUDE}/Graphics/Core/RenderTarget.hpp
		${CLV_INCLUDE}/Graphics/Core/Shader.hpp
		${CLV_INCLUDE}/Graphics/Core/ShaderTranspiler.hpp		
		${CLV_SOURCE}/Graphics/Core/ShaderTranspiler.cpp
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
		${CLV_INCLUDE}/Graphics/Direct3D/D3DGraphicsFactory.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DGraphicsFactory.cpp
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
		${CLV_INCLUDE}/Graphics/Metal/MTLCommandBuffer.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLCommandBuffer.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLPipelineObject.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLPipelineObject.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLGraphicsFactory.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLGraphicsFactory.mm
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
		${CLV_INCLUDE}/Graphics/OpenGL/GLGraphicsFactory.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLGraphicsFactory.cpp
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
		${CLV_INCLUDE}/Platform/Mac/CloveMac.hpp
		${CLV_INCLUDE}/Platform/Mac/MacPlatform.hpp
		${CLV_SOURCE}/Platform/Mac/MacPlatform.mm
		${CLV_INCLUDE}/Platform/Mac/MacWindow.hpp
		${CLV_SOURCE}/Platform/Mac/MacWindow.mm
)

set(
	CoreUtilityFiles
		${CLV_INCLUDE}/Core/Utils/Delegate.hpp
		${CLV_INCLUDE}/Core/Utils/Delegate.inl
		${CLV_INCLUDE}/Core/Utils/DeltaTime.hpp
		${CLV_SOURCE}/Core/Utils/DeltaTime.cpp
		${CLV_INCLUDE}/Core/Utils/MeshLoader.hpp
		${CLV_SOURCE}/Core/Utils/MeshLoader.cpp
		${CLV_INCLUDE}/Core/Utils/Timer.hpp
		${CLV_SOURCE}/Core/Utils/Timer.cpp
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