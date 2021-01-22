set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Graphics)
set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
	find_package(Vulkan REQUIRED)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(CLV_METAL_LIB Metal)
endif()

#Vulkan Implementation
set(
	VulkanFiles
		${INCLUDE}/Vulkan/DevicePointer.hpp
		${INCLUDE}/Vulkan/DevicePointer.inl
		${SOURCE}/Vulkan/DevicePointer.cpp
		${INCLUDE}/Vulkan/MemoryAllocator.hpp
		${INCLUDE}/Vulkan/MemoryAllocator.inl
		${SOURCE}/Vulkan/MemoryAllocator.cpp
		${INCLUDE}/Vulkan/VKBuffer.hpp
		${SOURCE}/Vulkan/VKBuffer.cpp
		${INCLUDE}/Vulkan/VKCommandBuffer.hpp
		${SOURCE}/Vulkan/VKCommandBuffer.cpp
		${INCLUDE}/Vulkan/VKDescriptorPool.hpp
		${INCLUDE}/Vulkan/VKDescriptorPool.inl
		${SOURCE}/Vulkan/VKDescriptorPool.cpp
		${INCLUDE}/Vulkan/VKDescriptorSet.hpp
		${SOURCE}/Vulkan/VKDescriptorSet.cpp
		${INCLUDE}/Vulkan/VKDescriptorSetLayout.hpp
		${INCLUDE}/Vulkan/VKDescriptorSetLayout.inl
		${SOURCE}/Vulkan/VKDescriptorSetLayout.cpp
		${INCLUDE}/Vulkan/VKFence.hpp
		${SOURCE}/Vulkan/VKFence.cpp
		${INCLUDE}/Vulkan/VKFramebuffer.hpp
		${SOURCE}/Vulkan/VKFramebuffer.cpp
		${INCLUDE}/Vulkan/VKGraphicsCommandBuffer.hpp
		${SOURCE}/Vulkan/VKGraphicsCommandBuffer.cpp
		${INCLUDE}/Vulkan/VKGraphicsDevice.hpp
		${SOURCE}/Vulkan/VKGraphicsDevice.cpp
		${INCLUDE}/Vulkan/VKGraphicsFactory.hpp
		${SOURCE}/Vulkan/VKGraphicsFactory.cpp
		${INCLUDE}/Vulkan/VKGraphicsQueue.hpp
		${SOURCE}/Vulkan/VKGraphicsQueue.cpp
		${INCLUDE}/Vulkan/VKGraphicsResource.hpp
		${SOURCE}/Vulkan/VKGraphicsResource.cpp
		${INCLUDE}/Vulkan/VKImage.hpp
		${INCLUDE}/Vulkan/VKImage.inl
		${SOURCE}/Vulkan/VKImage.cpp
		${INCLUDE}/Vulkan/VKImageView.hpp
		${INCLUDE}/Vulkan/VKImageView.inl
		${SOURCE}/Vulkan/VKImageView.cpp
		${INCLUDE}/Vulkan/VKMemoryBarrier.hpp
		${SOURCE}/Vulkan/VKMemoryBarrier.cpp
		${INCLUDE}/Vulkan/VKPipelineObject.hpp
		${SOURCE}/Vulkan/VKPipelineObject.cpp
		${INCLUDE}/Vulkan/VKPresentQueue.hpp
		${SOURCE}/Vulkan/VKPresentQueue.cpp
		${INCLUDE}/Vulkan/VKRenderPass.hpp
		${SOURCE}/Vulkan/VKRenderPass.cpp
		${INCLUDE}/Vulkan/VKResult.hpp
		${SOURCE}/Vulkan/VKResult.cpp
		${INCLUDE}/Vulkan/VKSampler.hpp
		${INCLUDE}/Vulkan/VKSampler.inl
		${SOURCE}/Vulkan/VKSampler.cpp
		${INCLUDE}/Vulkan/VKSemaphore.hpp
		${SOURCE}/Vulkan/VKSemaphore.cpp
		${INCLUDE}/Vulkan/VKShader.hpp
		${SOURCE}/Vulkan/VKShader.cpp
		${INCLUDE}/Vulkan/VKSwapchain.hpp
		${SOURCE}/Vulkan/VKSwapchain.cpp
		${INCLUDE}/Vulkan/VKTransferCommandBuffer.hpp
		${SOURCE}/Vulkan/VKTransferCommandBuffer.cpp
		${INCLUDE}/Vulkan/VKTransferQueue.hpp
		${SOURCE}/Vulkan/VKTransferQueue.cpp
		${INCLUDE}/Vulkan/VulkanTypes.hpp
)

#Library
add_library(
	CloveGraphics STATIC
		${INCLUDE}/CommandBuffer.hpp
		${INCLUDE}/Descriptor.hpp
		${INCLUDE}/GhaBuffer.hpp
		${INCLUDE}/GhaDescriptorPool.hpp
		${INCLUDE}/GhaDescriptorSet.hpp
		${INCLUDE}/GhaDescriptorSetLayout.hpp
		${INCLUDE}/GhaDevice.hpp
		${INCLUDE}/GhaFactory.hpp
		${INCLUDE}/GhaFence.hpp
		${INCLUDE}/GhaFramebuffer.hpp
		${INCLUDE}/GhaGraphicsCommandBuffer.hpp
		${INCLUDE}/GhaGraphicsQueue.hpp
		${INCLUDE}/GhaImage.hpp
		${INCLUDE}/GhaImageView.hpp
		${INCLUDE}/GhaPipelineObject.hpp
		${INCLUDE}/GhaPresentQueue.hpp
		${INCLUDE}/GhaRenderPass.hpp
		${INCLUDE}/GhaSampler.hpp
		${INCLUDE}/GhaSemaphore.hpp
		${INCLUDE}/GhaShader.hpp
		${INCLUDE}/GhaSwapchain.hpp
		${INCLUDE}/GhaTransferCommandBuffer.hpp
		${INCLUDE}/GhaTransferQueue.hpp
		${INCLUDE}/Graphics.hpp		
		${SOURCE}/Graphics.cpp
		${INCLUDE}/GraphicsAPI.hpp
		${INCLUDE}/MemoryBarrier.hpp
		${INCLUDE}/Queue.hpp
		${INCLUDE}/Resource.hpp
		${INCLUDE}/Result.hpp
		${INCLUDE}/ShaderCompiler.hpp		
		${SOURCE}/ShaderCompiler.cpp
		
		${VulkanFiles}

		#shaderc files required to use the file includer
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/glslc/file_includer.h
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/glslc/file_includer.cc

		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/libshaderc_util/file_finder.h
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/libshaderc_util/file_finder.cc
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/libshaderc_util/io.h
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/libshaderc_util/io.cc
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/libshaderc_util/string_piece.h
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc/libshaderc_util/universal_unistd.h
)

#Includes
target_include_directories(
    CloveGraphics
    
	PUBLIC
		include

	PRIVATE
		Vulkan::Vulkan
		${CMAKE_CURRENT_SOURCE_DIR}/external/shaderc
)

#Libraries
target_link_libraries(
    CloveGraphics
     
	PUBLIC
		CloveCast
		CloveDefinitions
		CloveExpected
		CloveLog
        CloveMaths

    PRIVATE
        CONAN_PKG::shaderc
		CONAN_PKG::spirv-cross

	#Windows
	PRIVATE
		$<$<PLATFORM_ID:Windows>:Vulkan::Vulkan>

	#MacOS
	PRIVATE
		$<$<PLATFORM_ID:Darwin>:${CLV_METAL_LIB}>

	#Linux
	PRIVATE
		$<$<PLATFORM_ID:Linux>:Vulkan::Vulkan>
)

#Don't compile files not supported on certain platforms
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_source_files_properties(${VulkanFiles} PROPERTIES HEADER_FILE_ONLY TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
endif()