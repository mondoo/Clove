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

		//TEMP
        clv::gfx::BufferDescriptor2 bufferDescriptor{};
        bufferDescriptor.size        = sizeof(Vertex) * std::size(vertices);
        bufferDescriptor.usageFlags  = clv::gfx::BufferUsageMode::VertexBuffer;
        bufferDescriptor.sharingMode = clv::gfx::BufferSharingMode::Exclusive;

        vertexBuffer = graphicsFactory->createBuffer(bufferDescriptor);
        vertexBuffer->map(std::data(vertices), bufferDescriptor.size);
        //~TEMP

        createPipeline();
        createSwapchainFrameBuffers();

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
            commandBuffers[i]->drawIndexed(std::size(vertices));
            commandBuffers[i]->endRenderPass();

            commandBuffers[i]->endRecording();
        }
    }
}