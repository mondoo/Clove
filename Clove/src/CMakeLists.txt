set(CLV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(CLV_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Clove)

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files
set(
	DelegateFiles
		${CLV_INCLUDE}/Delegate/DelegateHandle.hpp
		${CLV_SOURCE}/Delegate/DelegateHandle.cpp
		${CLV_INCLUDE}/Delegate/MultiCastDelegate.hpp
		${CLV_INCLUDE}/Delegate/MultiCastDelegate.inl
		${CLV_INCLUDE}/Delegate/SingleCastDelegate.hpp
		${CLV_INCLUDE}/Delegate/SingleCastDelegate.inl
)

set(
	EventFiles
		${CLV_INCLUDE}/Event/EventContainerBase.hpp
		${CLV_SOURCE}/Event/EventContainerBase.cpp
		${CLV_INCLUDE}/Event/EventDispatcher.hpp
		${CLV_INCLUDE}/Event/EventDispatcher.inl
		${CLV_SOURCE}/Event/EventDispatcher.cpp
		${CLV_INCLUDE}/Event/EventHandle.hpp
		${CLV_SOURCE}/Event/EventHandle.cpp
		${CLV_INCLUDE}/Event/EventManager.hpp
		${CLV_INCLUDE}/Event/EventManager.inl
		${CLV_SOURCE}/Event/EventManager.cpp
		${CLV_INCLUDE}/Event/EventTypes.hpp
)

set(
	ExceptionFiles
		${CLV_INCLUDE}/Exception/CloveException.hpp
		${CLV_SOURCE}/Exception/CloveException.cpp
)

set(
	GraphicsFiles
		${CLV_INCLUDE}/Graphics/Buffer.hpp
		${CLV_INCLUDE}/Graphics/Texture.hpp
		${CLV_INCLUDE}/Graphics/CommandBuffer.hpp
		${CLV_INCLUDE}/Graphics/FactoryChild.hpp
		${CLV_INCLUDE}/Graphics/Graphics.hpp		
		${CLV_SOURCE}/Graphics/Graphics.cpp
		${CLV_INCLUDE}/Graphics/GraphicsTypes.hpp
		${CLV_INCLUDE}/Graphics/PipelineObject.hpp
		${CLV_INCLUDE}/Graphics/GraphicsFactory.hpp
		${CLV_INCLUDE}/Graphics/RenderTarget.hpp
		${CLV_INCLUDE}/Graphics/Shader.hpp
		${CLV_INCLUDE}/Graphics/ShaderTranspiler.hpp		
		${CLV_SOURCE}/Graphics/ShaderTranspiler.cpp
		${CLV_INCLUDE}/Graphics/Surface.hpp
		${CLV_INCLUDE}/Graphics/VertexLayout.hpp
		${CLV_INCLUDE}/Graphics/VertexLayout.inl
		${CLV_SOURCE}/Graphics/VertexLayout.cpp	
)

set(
	Direct3DGraphicsFiles
		${CLV_INCLUDE}/Graphics/Direct3D/D3DBuffer.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DBuffer.cpp
		${CLV_INCLUDE}/Graphics/Direct3D/D3DTexture.hpp
		${CLV_SOURCE}/Graphics/Direct3D/D3DTexture.cpp
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
		${CLV_INCLUDE}/Graphics/Metal/MTLBuffer.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLBuffer.mm
		${CLV_INCLUDE}/Graphics/Metal/MTLTexture.hpp
		${CLV_SOURCE}/Graphics/Metal/MTLTexture.mm
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
		${CLV_INCLUDE}/Graphics/OpenGL/GLBuffer.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLBuffer.cpp
		${CLV_INCLUDE}/Graphics/OpenGL/GLTexture.hpp
		${CLV_SOURCE}/Graphics/OpenGL/GLTexture.cpp
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
	MemoryFiles
		${CLV_INCLUDE}/Memory/ListAllocator.hpp
		${CLV_SOURCE}/Memory/ListAllocator.cpp
		${CLV_SOURCE}/Memory/AllocationTracker.cpp
		${CLV_INCLUDE}/Memory/PoolAllocator.hpp
		${CLV_INCLUDE}/Memory/PoolAllocator.inl
		${CLV_INCLUDE}/Memory/StackAllocator.hpp
		${CLV_SOURCE}/Memory/StackAllocator.cpp
)

set(
	InputFiles
		${CLV_INCLUDE}/Input/InputEvent.hpp
		${CLV_INCLUDE}/Input/Keyboard.hpp
		${CLV_INCLUDE}/Input/Keyboard.inl
		${CLV_SOURCE}/Input/Keyboard.cpp
		${CLV_INCLUDE}/Input/KeyCodes.hpp
		${CLV_INCLUDE}/Input/Mouse.hpp
		${CLV_SOURCE}/Input/Mouse.cpp
		${CLV_INCLUDE}/Input/MouseButtonCodes.hpp
)

set(
	MathsFiles
		${CLV_INCLUDE}/Maths/glmWrappers.hpp
		${CLV_INCLUDE}/Maths/Maths.hpp
		${CLV_INCLUDE}/Maths/Maths.inl
		${CLV_INCLUDE}/Maths/MathsHelpers.hpp
		${CLV_INCLUDE}/Maths/MathsHelpers.inl
		${CLV_INCLUDE}/Maths/MathsTypes.hpp
		${CLV_INCLUDE}/Maths/Matrix.hpp
		${CLV_INCLUDE}/Maths/Quaternion.hpp
		${CLV_INCLUDE}/Maths/Vector.hpp
)

set(
	PlatformFiles
		${CLV_INCLUDE}/Platform/Platform.hpp
		${CLV_SOURCE}/Platform/Platform.cpp
		${CLV_INCLUDE}/Platform/PlatformTypes.hpp
		${CLV_INCLUDE}/Platform/Window.hpp
		${CLV_SOURCE}/Platform/Window.cpp
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
	UtilityFiles
		${CLV_INCLUDE}/Utils/Cast.hpp
		${CLV_INCLUDE}/Utils/Cast.inl
		${CLV_INCLUDE}/Utils/DeltaTime.hpp
		${CLV_SOURCE}/Utils/DeltaTime.cpp
		${CLV_INCLUDE}/Utils/Timer.hpp
		${CLV_SOURCE}/Utils/Timer.cpp
)

set(
	AllFiles
	${CLV_INCLUDE}/Clove.hpp
	${CLV_INCLUDE}/Core.hpp
	${CLV_INCLUDE}/Log.hpp
	${CLV_INCLUDE}/Log.inl
	${CLV_SOURCE}/Log.cpp

		${DelegateFiles}
		${EventFiles}
		${ExceptionFiles}
		${GraphicsFiles}
			${OpenGLGraphicsFiles}
				${WindowsGLGraphicsFiles}
				${LinuxGLGraphicsFiles}
			${Direct3DGraphicsFiles}
			${MetalGraphicsFiles}
		${MemoryFiles}
		${InputFiles}
		${MathsFiles}
		${PlatformFiles}
			${WindowsPlatformFiles}
			${LinuxPlatformFiles}
			${MacOSPlatformFiles}
		${UtilityFiles}
)

#Organise the source tree
file(GLOB_RECURSE header ${CLV_INCLUDE}/*.h ${CLV_INCLUDE}/*.hpp ${CLV_INCLUDE}/*.inl)
file(GLOB_RECURSE source *.cpp *.m *.mm)
source_group(TREE ${CLV_INCLUDE} FILES ${header})
source_group(TREE ${CLV_SOURCE} FILES ${source})

source_group(PCH FILES 
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