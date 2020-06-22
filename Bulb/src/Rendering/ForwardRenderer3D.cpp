#include "Bulb/Rendering/ForwardRenderer3D.hpp"

#include <Clove/Platform/Window.hpp>

namespace blb::rnd {
	ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API) {
		//TODO: Add an abstraction for creating the factory
		graphicsFactory = std::make_shared<clv::gfx::vk::VKGraphicsFactory>(window.getNativeWindow());

		graphicsQueue = graphicsFactory->createGraphicsQueue({ clv::gfx::QueueFlags::None });
		presentQueue  = graphicsFactory->createPresentQueue();
		transferQueue = graphicsFactory->createTransferQueue({ clv::gfx::QueueFlags::Transient });

		swapchain = graphicsFactory->createSwapChain({ window.getSize() });

		std::shared_ptr<clv::gfx::vk::VKShader> vertShader = graphicsFactory->createShader("vert.spirv");
		std::shared_ptr<clv::gfx::vk::VKShader> fragShader = graphicsFactory->createShader("frag.spirv");

		std::shared_ptr<clv::gfx::vk::VKRenderPass> renderPass = graphicsFactory->createRenderPass({ swapchain->getImageFormat() });

		clv::gfx::PiplineObjectDescriptor pipelineDescriptor;
		pipelineDescriptor.vertexShader			   = vertShader;
		pipelineDescriptor.fragmentShader		   = fragShader;
		pipelineDescriptor.viewportDescriptor.size = { swapchain->getExtent().width, swapchain->getExtent().height };
		pipelineDescriptor.scissorDescriptor.size  = { swapchain->getExtent().width, swapchain->getExtent().height };
		pipelineDescriptor.renderPass			   = renderPass;

		pipelineObject = graphicsFactory->createPipelineObject(pipelineDescriptor);

		for(auto& swapChainImageView : swapchain->getImageViews()) {
			clv::gfx::FramebufferDescriptor frameBufferDescriptor{};
			frameBufferDescriptor.renderPass  = renderPass;
			frameBufferDescriptor.attachments = { swapChainImageView };
			frameBufferDescriptor.width		  = swapchain->getExtent().width;
			frameBufferDescriptor.height	  = swapchain->getExtent().height;

			swapChainFrameBuffers.emplace_back(graphicsFactory->createFramebuffer(frameBufferDescriptor));
		}

		commandBuffers.reserve(swapChainFrameBuffers.size());
		for(size_t i = 0; i < swapChainFrameBuffers.size(); ++i) {
			commandBuffers.emplace_back(graphicsQueue->allocateCommandBuffer());
		}

		clv::gfx::RenderArea renderArea{};
		renderArea.origin = { 0, 0 };
		renderArea.size	  = {swapchain->getExtent().width, swapchain->getExtent().height };

		clv::mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };

		//Record our command buffers
		for(size_t i = 0; i < commandBuffers.size(); ++i) {
			commandBuffers[i]->beginRecording();

			commandBuffers[i]->beginRenderPass(*renderPass, *swapChainFrameBuffers[i], renderArea, clearColour);
			commandBuffers[i]->bindPipelineObject(*pipelineObject);
			commandBuffers[i]->drawIndexed();
			commandBuffers[i]->endRenderPass();

			commandBuffers[i]->endRecording();
		}

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
		//Wait for all of our fences before shutting down so we don't delete objects while vulkan is using them
		for(auto& fence : inFlightFences) {
			fence->waitForFence();
		}
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
		inFlightFences[currentFrame]->waitForFence();

		//Aquire the next available image
		uint32_t imageIndex = swapchain->aquireNextImage(imageAvailableSemaphores[currentFrame].get());

		//Check if we're already using this image, if so wait
		if(imagesInFlight[imageIndex] != nullptr) {
			imagesInFlight[imageIndex]->waitForFence();
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame]; //Ptr copy here, could be slowing things down

		inFlightFences[currentFrame]->resetFence();

		//Submit that command buffer associated with that image
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
		presentQueue->present(presentInfo);

		currentFrame = (currentFrame + 1) % maxFramesInFlight;
	}

	void ForwardRenderer3D::recreateSwapchain() {

	}
}