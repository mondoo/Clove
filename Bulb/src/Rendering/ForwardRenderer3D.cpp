#include "Bulb/Rendering/ForwardRenderer3D.hpp"

#include <Clove/Platform/Window.hpp>

namespace blb::rnd {
	ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API) {
		windowResizeHandle = window.onWindowResize.bind(&ForwardRenderer3D::onWindowResize, this);
        windowSize         = window.getSize();

		//TODO: Add an abstraction for creating the factory
		graphicsFactory = std::make_shared<clv::gfx::vk::VKGraphicsFactory>(window.getNativeWindow());

		graphicsQueue = graphicsFactory->createGraphicsQueue({ clv::gfx::QueueFlags::None });
		presentQueue  = graphicsFactory->createPresentQueue();
		transferQueue = graphicsFactory->createTransferQueue({ clv::gfx::QueueFlags::Transient });

		swapchain  = graphicsFactory->createSwapChain({ windowSize });
        renderPass = graphicsFactory->createRenderPass({ swapchain->getImageFormat() });

        //TODO: Retrieve these from the shaders
        clv::gfx::DescriptorBindingInfo descriptorBinding{};
        descriptorBinding.binding   = 0;
        descriptorBinding.type      = clv::gfx::DescriptorType::UniformBuffer;
        descriptorBinding.arraySize = 1;
        descriptorBinding.stage     = clv::gfx::DescriptorStage::Vertex;

        clv::gfx::DescriptorSetLayoutDescriptor descriptorSetLayoutDescriptor{};
        descriptorSetLayoutDescriptor.bindings.push_back(descriptorBinding);

        descriptorSetLayout = graphicsFactory->createDescriptorSetLayout(descriptorSetLayoutDescriptor);

		//TEMP - Testing drawing
        const size_t vertexBufferSize = sizeof(Vertex) * std::size(vertices);
        const size_t indexBufferSize  = sizeof(uint16_t) * std::size(indices);

        //Staging buffers
        clv::gfx::BufferDescriptor2 stagingDescriptor{};
        stagingDescriptor.usageFlags       = clv::gfx::BufferUsageMode::TransferSource;
        stagingDescriptor.sharingMode      = clv::gfx::SharingMode::Exclusive; //Only accessed by the transfer queue
        stagingDescriptor.memoryProperties = clv::gfx::MemoryProperties::HostVisible;

        stagingDescriptor.size   = vertexBufferSize;
        auto stagingBufferVertex = graphicsFactory->createBuffer(stagingDescriptor);

        stagingDescriptor.size  = indexBufferSize;
        auto stagingBufferIndex = graphicsFactory->createBuffer(stagingDescriptor);

        auto stagingBuffer = graphicsFactory->createBuffer(stagingDescriptor);

        //Vertex buffer
        clv::gfx::BufferDescriptor2 vertexDescriptor{};
        vertexDescriptor.size             = vertexBufferSize;
        vertexDescriptor.usageFlags       = clv::gfx::BufferUsageMode::TransferDestination | clv::gfx::BufferUsageMode::VertexBuffer;
        vertexDescriptor.sharingMode      = clv::gfx::SharingMode::Concurrent;//Accessed by transfer and graphics queue
        vertexDescriptor.memoryProperties = clv::gfx::MemoryProperties::DeviceLocal;

        vertexBuffer = graphicsFactory->createBuffer(vertexDescriptor);

        //Index Buffer
        clv::gfx::BufferDescriptor2 indexDescriptor{};
        indexDescriptor.size             = indexBufferSize;
        indexDescriptor.usageFlags       = clv::gfx::BufferUsageMode::TransferDestination | clv::gfx::BufferUsageMode::IndexBuffer;
        indexDescriptor.sharingMode      = clv::gfx::SharingMode::Concurrent;
        indexDescriptor.memoryProperties = clv::gfx::MemoryProperties::DeviceLocal;

        indexBuffer = graphicsFactory->createBuffer(indexDescriptor);

        //Texture
        //TODO...
        
        //Map the data onto our CPU visible buffer
        stagingBufferVertex->map(std::data(vertices), vertexBufferSize);
        stagingBufferIndex->map(std::data(indices), indexBufferSize);       

        std::shared_ptr<clv::gfx::vk::VKTransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();
        transferCommandBuffer->beginRecording(clv::gfx::CommandBufferUsage::OneTimeSubmit);
        transferCommandBuffer->copyBufferToBuffer(*stagingBufferVertex, 0, *vertexBuffer, 0, vertexBufferSize);
        transferCommandBuffer->copyBufferToBuffer(*stagingBufferIndex, 0, *indexBuffer, 0, indexBufferSize);
        transferCommandBuffer->endRecording();
        transferQueue->submit({ { transferCommandBuffer } });
        transferQueue->freeCommandBuffer(*transferCommandBuffer);
        

        //Adding a texture

        //~TEMP

        createPipeline();
        createSwapchainFrameBuffers();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();

        recordCommandBuffers();

        needNewSwapchain = false;

		//Create semaphores for frame synchronisation
		for(auto& renderFinishedSemaphore : renderFinishedSemaphores) {
			renderFinishedSemaphore = graphicsFactory->createSemaphore();
		}
		for(auto& imageAvailableSemaphore : imageAvailableSemaphores) {
			imageAvailableSemaphore = graphicsFactory->createSemaphore();
		}
		for(auto& inFlightFence : inFlightFences) {
			inFlightFence = graphicsFactory->createFence({ true });
		}
		imagesInFlight.resize(swapchain->getImageViews().size());
	}

	ForwardRenderer3D::~ForwardRenderer3D(){
		//Wait for an idle device before shutting down so resources aren't freed while in use
		graphicsFactory->waitForIdleDevice();

        //Reset buffer manually to ensure correct destruction order
        vertexBuffer.reset();
        indexBuffer.reset();
        uniformBuffers.clear();
	}

	void ForwardRenderer3D::begin() {
		//TODO
	}

	void ForwardRenderer3D::submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) {
	}

	void ForwardRenderer3D::submitLight(const DirectionalLight& light) {
	}

	void ForwardRenderer3D::submitLight(const PointLight& light) {
	}

	void ForwardRenderer3D::submitCamera(const ComposedCameraData& camera) {
	}

	void ForwardRenderer3D::submitWidget(const std::shared_ptr<Sprite>& widget) {
	}

	void ForwardRenderer3D::submitText(const std::shared_ptr<Sprite>& text) {
	}

	void ForwardRenderer3D::end() {
        if(needNewSwapchain) {
            recreateSwapchain();
            return;
        }

		inFlightFences[currentFrame]->waitForFence();

		//Aquire the next available image
		uint32_t imageIndex;
		clv::gfx::Result result = swapchain->aquireNextImage(imageAvailableSemaphores[currentFrame].get(), imageIndex);
        if(result == clv::gfx::Result::Error_SwapchainOutOfDate) {
            recreateSwapchain();
            return;
        }

		//Check if we're already using this image, if so wait
		if(imagesInFlight[imageIndex] != nullptr) {
			imagesInFlight[imageIndex]->waitForFence();
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame]; //Ptr copy here, could be slowing things down

		inFlightFences[currentFrame]->resetFence();

        //Update our UBOs
        updateUniformBuffer(imageIndex);

		//Submit the command buffer associated with that image
		clv::gfx::GraphicsSubmitInfo submitInfo{};
		submitInfo.waitSemaphores	= { imageAvailableSemaphores[currentFrame] };
		submitInfo.waitStages		= { clv::gfx::WaitStage::ColourAttachmentOutput };
		submitInfo.commandBuffers	= { commandBuffers[imageIndex] };
		submitInfo.signalSemaphores = { renderFinishedSemaphores[currentFrame] };
		graphicsQueue->submit(submitInfo, inFlightFences[currentFrame]);

		//Present current image
		clv::gfx::PresentInfo presentInfo{};
		presentInfo.waitSemaphores = { renderFinishedSemaphores[currentFrame] };
		presentInfo.swapChain	   = swapchain;
		presentInfo.imageIndex	   = imageIndex;
        result = presentQueue->present(presentInfo);
        if(needNewSwapchain || result == clv::gfx::Result::Error_SwapchainOutOfDate || result == clv::gfx::Result::Success_SwapchainSuboptimal) {
            recreateSwapchain();
        }

		currentFrame = (currentFrame + 1) % maxFramesInFlight;
	}

	void ForwardRenderer3D::onWindowResize(const clv::mth::vec2ui& size) {
        windowSize = size;
        needNewSwapchain = true;
	}

	void ForwardRenderer3D::recreateSwapchain() {
		//TODO: Clean this up

		//Set this to true in case we need to wait for the window to unminimise
		needNewSwapchain = true;

		if(windowSize.x == 0 || windowSize.y == 0) {
            return;
		}

        graphicsFactory->waitForIdleDevice();

		//Explicitly free resources to avoid problems when recreating the swap chain itself
        swapchain.reset();
        pipelineObject.reset();
        swapChainFrameBuffers.clear();
        commandBuffers.clear();

        //Recreate our swap chain
        swapchain = graphicsFactory->createSwapChain({ windowSize });
        renderPass = graphicsFactory->createRenderPass({ swapchain->getImageFormat() });

        createPipeline();
        createSwapchainFrameBuffers();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        
		recordCommandBuffers();

		needNewSwapchain = false;
    }

    void ForwardRenderer3D::createPipeline() {
        clv::gfx::PiplineObjectDescriptor pipelineDescriptor;
        pipelineDescriptor.vertexShader            = graphicsFactory->createShader("vert.spirv");
        pipelineDescriptor.fragmentShader          = graphicsFactory->createShader("frag.spirv");
        pipelineDescriptor.vertexInput             = Vertex::getInputBindingDescriptor();
        pipelineDescriptor.vertexAttributes        = Vertex::getVertexAttributes();
        pipelineDescriptor.viewportDescriptor.size = { swapchain->getExtent().width, swapchain->getExtent().height };
        pipelineDescriptor.scissorDescriptor.size  = { swapchain->getExtent().width, swapchain->getExtent().height };
        pipelineDescriptor.renderPass              = renderPass;
        pipelineDescriptor.descriptorSetLayouts    = { descriptorSetLayout };

        pipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);
    }

	void ForwardRenderer3D::createSwapchainFrameBuffers() {
        for(auto& swapChainImageView : swapchain->getImageViews()) {
            clv::gfx::FramebufferDescriptor frameBufferDescriptor{};
            frameBufferDescriptor.renderPass  = renderPass;
            frameBufferDescriptor.attachments = { swapChainImageView };
            frameBufferDescriptor.width       = swapchain->getExtent().width;
            frameBufferDescriptor.height      = swapchain->getExtent().height;

            swapChainFrameBuffers.emplace_back(graphicsFactory->createFramebuffer(frameBufferDescriptor));
        }
    }

    void ForwardRenderer3D::createUniformBuffers() {
        const size_t frameBufferNum = std::size(swapChainFrameBuffers);
        uniformBuffers.resize(frameBufferNum);

        for(size_t i = 0; i < frameBufferNum; ++i) {
            clv::gfx::BufferDescriptor2 descriptor{};
            descriptor.size             = sizeof(ModelViewProj);
            descriptor.usageFlags       = clv::gfx::BufferUsageMode::UniformBuffer;
            descriptor.sharingMode      = clv::gfx::SharingMode::Exclusive;
            descriptor.memoryProperties = clv::gfx::MemoryProperties::HostVisible;

            uniformBuffers[i] = graphicsFactory->createBuffer(std::move(descriptor));
        }
    }

    void ForwardRenderer3D::createDescriptorPool() {
        clv::gfx::DescriptorInfo descriptorInfo{};
        descriptorInfo.type  = clv::gfx::DescriptorType::UniformBuffer;
        descriptorInfo.count = static_cast<uint32_t>(std::size(swapChainFrameBuffers));

        clv::gfx::DescriptorPoolDescriptor poolDescriptor{};
        poolDescriptor.poolTypes.emplace_back(std::move(descriptorInfo));
        poolDescriptor.maxSets = static_cast<uint32_t>(std::size(swapChainFrameBuffers));

        descriptorPool = graphicsFactory->createDescriptorPool(std::move(poolDescriptor));
    }

    void ForwardRenderer3D::createDescriptorSets() {
        //We need to make one set for each frame-in-flight, so we duplicate the layout we havbe
        std::vector<std::shared_ptr<clv::gfx::vk::VKDescriptorSetLayout>> layouts(std::size(swapChainFrameBuffers), descriptorSetLayout);
        
        descriptorSets = descriptorPool->allocateDescriptorSets(layouts);

        for(size_t i = 0; i < std::size(swapChainFrameBuffers); ++i) {
            descriptorSets[i]->writeFrom(*uniformBuffers[i], 0, sizeof(ModelViewProj), 0);
        }
    }

	void blb::rnd::ForwardRenderer3D::recordCommandBuffers() {
        commandBuffers.reserve(swapChainFrameBuffers.size());
        for(size_t i = 0; i < swapChainFrameBuffers.size(); ++i) {
            commandBuffers.emplace_back(graphicsQueue->allocateCommandBuffer());
        }

        clv::gfx::RenderArea renderArea{};
        renderArea.origin = { 0, 0 };
        renderArea.size   = { swapchain->getExtent().width, swapchain->getExtent().height };

        clv::mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };

        //Record our command buffers
        for(size_t i = 0; i < commandBuffers.size(); ++i) {
            commandBuffers[i]->beginRecording(clv::gfx::CommandBufferUsage::Default);

            commandBuffers[i]->beginRenderPass(*renderPass, *swapChainFrameBuffers[i], renderArea, clearColour);
            commandBuffers[i]->bindPipelineObject(*pipelineObject);
            commandBuffers[i]->bindVertexBuffer(*vertexBuffer);
            commandBuffers[i]->bindIndexBuffer(*indexBuffer, clv::gfx::IndexType::Uint16);
            commandBuffers[i]->bindDescriptorSet(*descriptorSets[i], *pipelineObject);
            commandBuffers[i]->drawIndexed(std::size(indices));
            commandBuffers[i]->endRenderPass();

            commandBuffers[i]->endRecording();
        }
    }

    void ForwardRenderer3D::updateUniformBuffer(const size_t imageIndex) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        ModelViewProj ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view  = glm::lookAt(glm::vec3(0.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.proj  = glm::perspective(glm::radians(45.0f), static_cast<float>(swapchain->getExtent().width) / static_cast<float>(swapchain->getExtent().height), 0.1f, 10.0f);

        uniformBuffers[imageIndex]->map(&ubo, sizeof(ubo));
    }
}