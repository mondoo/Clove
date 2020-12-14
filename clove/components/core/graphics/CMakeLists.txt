set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/Clove/Graphics)
set(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_subdirectory(external)

find_package(Vulkan REQUIRED)

#Library
add_library(
	CloveGraphics STATIC
		${INCLUDE}/CommandBuffer.hpp
		${INCLUDE}/Descriptor.hpp
		${INCLUDE}/DescriptorPool.hpp
		${INCLUDE}/DescriptorSet.hpp
		${INCLUDE}/DescriptorSetLayout.hpp
		${INCLUDE}/Fence.hpp
		${INCLUDE}/Framebuffer.hpp
		${INCLUDE}/Graphics.hpp		
		${SOURCE}/Graphics.cpp
		${INCLUDE}/GraphicsAPI.hpp
		${INCLUDE}/GraphicsBuffer.hpp
		${INCLUDE}/GraphicsCommandBuffer.hpp
		${INCLUDE}/GraphicsDevice.hpp
		${INCLUDE}/GraphicsFactory.hpp
		${INCLUDE}/GraphicsImage.hpp
		${INCLUDE}/GraphicsImageView.hpp
		${INCLUDE}/GraphicsQueue.hpp
		${INCLUDE}/GraphicsResource.hpp
		${INCLUDE}/MemoryBarrier.hpp
		${INCLUDE}/PipelineObject.hpp
		${INCLUDE}/PresentQueue.hpp
		${INCLUDE}/Queue.hpp
		${INCLUDE}/RenderPass.hpp
		${INCLUDE}/Result.hpp
		${INCLUDE}/Sampler.hpp
		${INCLUDE}/Semaphore.hpp
		${INCLUDE}/Shader.hpp
		${INCLUDE}/ShaderCompiler.hpp		
		${SOURCE}/ShaderCompiler.cpp
		${INCLUDE}/Swapchain.hpp
		${INCLUDE}/TransferCommandBuffer.hpp
        ${INCLUDE}/TransferQueue.hpp
        
        #Vulkan
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

#Includes
target_include_directories(
    CloveGraphics
    
	PUBLIC
		include

	PRIVATE
		Vulkan::Vulkan
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
        shaderc
		SPIRV
		spirv-cross-core
		spirv-cross-glsl
		spirv-cross-hlsl
		spirv-cross-msl

	#Windows
	PRIVATE
		$<$<PLATFORM_ID:Windows>:Vulkan::Vulkan>

	#Linux
	PRIVATE
		$<$<PLATFORM_ID:Linux>:Vulkan::Vulkan>
)