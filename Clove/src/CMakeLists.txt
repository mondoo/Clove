set(CLV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(CLV_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include/Clove)

project(Clove)

add_library(${PROJECT_NAME} STATIC)

#Files
set(
	AudioFiles
		${CLV_INCLUDE}/Audio/Audio.hpp
		${CLV_SOURCE}/Audio/Audio.cpp
		${CLV_INCLUDE}/Audio/AudioBuffer.hpp
		${CLV_INCLUDE}/Audio/AudioFactory.hpp
		${CLV_INCLUDE}/Audio/AudioListener.hpp
		${CLV_INCLUDE}/Audio/AudioSource.hpp	
)

set(
	OpenALAudioFiles
		${CLV_INCLUDE}/Audio/OpenAL/ALBuffer.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALBuffer.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALError.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALError.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALFactory.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALFactory.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALListener.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALListener.cpp
		${CLV_INCLUDE}/Audio/OpenAL/ALSource.hpp
		${CLV_SOURCE}/Audio/OpenAL/ALSource.cpp
)

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
		${CLV_INCLUDE}/Graphics/DescriptorSetLayout.hpp
		${CLV_INCLUDE}/Graphics/Fence.hpp
		${CLV_INCLUDE}/Graphics/Framebuffer.hpp
		${CLV_INCLUDE}/Graphics/Graphics.hpp		
		${CLV_SOURCE}/Graphics/Graphics.cpp
		${CLV_INCLUDE}/Graphics/GraphicsBuffer.hpp
		${CLV_INCLUDE}/Graphics/GraphicsFactory.hpp
		${CLV_INCLUDE}/Graphics/GraphicsImage.hpp
		${CLV_INCLUDE}/Graphics/GraphicsImageView.hpp
		${CLV_INCLUDE}/Graphics/GraphicsTypes.hpp
		${CLV_INCLUDE}/Graphics/RenderPass.hpp
		${CLV_INCLUDE}/Graphics/Sampler.hpp
		${CLV_INCLUDE}/Graphics/Semaphore.hpp
		${CLV_INCLUDE}/Graphics/Shader.hpp
		${CLV_INCLUDE}/Graphics/ShaderTranspiler.hpp		
		${CLV_SOURCE}/Graphics/ShaderTranspiler.cpp
		${CLV_INCLUDE}/Graphics/Swapchain.hpp
)

set(
	VulkanGraphicsFiles
		${CLV_INCLUDE}/Graphics/Vulkan/VKBuffer.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKBuffer.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKCommandBuffer.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKCommandBuffer.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKCommandQueue.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKCommandQueue.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKDescriptorPool.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKDescriptorPool.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKDescriptorSet.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKDescriptorSet.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKDescriptorSetLayout.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKDescriptorSetLayout.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKFence.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKFence.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKFramebuffer.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKFramebuffer.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKPipelineObject.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKPipelineObject.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKRenderPass.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKRenderPass.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKSampler.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKSampler.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKSemaphore.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKSemaphore.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKGraphicsFactory.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKGraphicsFactory.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKImage.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKImage.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKImageView.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKImageView.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKShader.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKShader.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VKSwapchain.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VKSwapchain.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VulkanHelpers.hpp
		${CLV_SOURCE}/Graphics/Vulkan/VulkanHelpers.cpp
		${CLV_INCLUDE}/Graphics/Vulkan/VulkanTypes.hpp
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
	${CLV_INCLUDE}/Log.hpp
	${CLV_INCLUDE}/Log.inl
	${CLV_SOURCE}/Log.cpp

		${AudioFiles}
			${OpenALAudioFiles}
		${DelegateFiles}
		${EventFiles}
		${ExceptionFiles}
		${GraphicsFiles}
			${VulkanGraphicsFiles}
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
	set_source_files_properties(${LinuxPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_source_files_properties(${WindowsPlatformFiles} ${MacOSPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set_source_files_properties(${WindowsPlatformFiles} ${LinuxPlatformFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()