#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

//TODO: Move to cpp
#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"
//TEMP
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"

namespace clv::plt {
	class Window;
}

namespace clv::gfx {
	class RenderTarget;
}

namespace blb::rnd {
	class Sprite;
	class Mesh;
}

namespace blb::rnd {
	class IRenderer3D {//Temp IRenderer3D until old renderer has been removed
					   //TYPES
	public:
		struct ComposedCameraData {
			clv::gfx::Viewport viewport;
			CameraRenderData bufferData;
			std::shared_ptr<clv::gfx::RenderTarget> target;
		};

		//FUNCTIONS
	public:
		virtual void begin() = 0;

		virtual void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) = 0;
		virtual void submitLight(const DirectionalLight& light)			= 0;
		virtual void submitLight(const PointLight& light)				= 0;
		virtual void submitCamera(const ComposedCameraData& camera)		= 0;

		virtual void submitWidget(const std::shared_ptr<Sprite>& widget) = 0;
		virtual void submitText(const std::shared_ptr<Sprite>& text)	 = 0;

		virtual void end() = 0;
	};
}

#include "Bulb/Rendering/RenderingTypes.hpp"

#include <Clove/Graphics/Vulkan/VKGraphicsFactory.hpp>

namespace blb::rnd {
	class ForwardRenderer3D : public IRenderer3D {
		//VARIABLES
	private:
		//TEMP
		struct ModelViewProj{
            clv::mth::mat4f model;
            clv::mth::mat4f view;
            clv::mth::mat4f proj;
		};
		//We have an array of these because we basically have a UBO per frame in flight
		//TODO: How would we control this for the client? Would they need to request a UBO each frame?
		//	Mesh, Sprite, Material etc. Would need to be set up specifically with this in mind.
		//	Perhaps they can store the data and sort out the buffers when given to the renderer
        std::vector<std::shared_ptr<clv::gfx::vk::VKBuffer>> uniformBuffers;
        
        const std::vector<Vertex> vertices = {
            { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
            { {  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
            { {  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
            { { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
        };
        const std::vector<uint16_t> indices = {
            0,
            1,
            2,
            2,
            3,
            0
        };
        std::shared_ptr<clv::gfx::vk::VKBuffer> vertexBuffer;
        std::shared_ptr<clv::gfx::vk::VKBuffer> indexBuffer;
		//~TEMP

		clv::DelegateHandle windowResizeHandle;
        clv::mth::vec2ui windowSize;
        bool needNewSwapchain = false;

		static constexpr size_t maxFramesInFlight = 2;
		size_t currentFrame						  = 0;

		//Using shared_ptrs of the actual types until API is abstracted
		std::shared_ptr<clv::gfx::vk::VKGraphicsFactory> graphicsFactory;

		std::shared_ptr<clv::gfx::vk::VKGraphicsQueue> graphicsQueue;
		std::shared_ptr<clv::gfx::vk::VKPresentQueue> presentQueue;
		std::shared_ptr<clv::gfx::vk::VKTransferQueue> transferQueue;

		std::shared_ptr<clv::gfx::vk::VKSwapchain> swapchain;

		std::shared_ptr<clv::gfx::vk::VKRenderPass> renderPass;
        std::shared_ptr<clv::gfx::vk::VKDescriptorSetLayout> descriptorSetLayout;

		std::shared_ptr<clv::gfx::vk::VKPipelineObject> pipelineObject;

		std::vector<std::shared_ptr<clv::gfx::vk::VKFramebuffer>> swapChainFrameBuffers;

		std::vector<std::shared_ptr<clv::gfx::vk::VKGraphicsCommandBuffer>> commandBuffers;

		std::array<std::shared_ptr<clv::gfx::vk::VKSemaphore>, maxFramesInFlight> renderFinishedSemaphores;
		std::array<std::shared_ptr<clv::gfx::vk::VKSemaphore>, maxFramesInFlight> imageAvailableSemaphores;
		std::array<std::shared_ptr<clv::gfx::vk::VKFence>, maxFramesInFlight> inFlightFences;
		std::vector<std::shared_ptr<clv::gfx::vk::VKFence>> imagesInFlight;

		//FUNCTIONS
	public:
		//TODO: Ctors
		ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API);
		~ForwardRenderer3D();

		void begin() final;

		void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh) final;
		void submitLight(const DirectionalLight& light) final;
		void submitLight(const PointLight& light) final;
		void submitCamera(const ComposedCameraData& camera) final;

		void submitWidget(const std::shared_ptr<Sprite>& widget) final;
		void submitText(const std::shared_ptr<Sprite>& text) final;

		void end() final;

	private:
        void onWindowResize(const clv::mth::vec2ui& size);
		void recreateSwapchain();

		void createPipeline();
        void createSwapchainFrameBuffers();
        void createUniformBuffers();
        void recordCommandBuffers();

	};
}