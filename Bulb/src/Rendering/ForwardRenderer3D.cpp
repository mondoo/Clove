#include "Bulb/Rendering/ForwardRenderer3D.hpp"

#include <Clove/Platform/Window.hpp>

namespace blb::rnd {
	ForwardRenderer3D::ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API) {
		graphicsFactory = std::make_shared<clv::gfx::vk::VKGraphicsFactory>(window.getNativeWindow());

		//Retrieving data as a test
		graphicsQueue = graphicsFactory->createGraphicsQueue({ clv::gfx::QueueFlags::None });
		presentQueue  = graphicsFactory->createPresentQueue();
		transferQueue = graphicsFactory->createTransferQueue({ clv::gfx::QueueFlags::Transient });

		swapchain = graphicsFactory->createSwapChain({});

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
	}

	ForwardRenderer3D::~ForwardRenderer3D() = default;

	void ForwardRenderer3D::begin() {
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
	}
}