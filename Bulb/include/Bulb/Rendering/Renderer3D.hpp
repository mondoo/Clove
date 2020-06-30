#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>
#include <Clove/Graphics/VertexLayout.hpp>

namespace clv::gfx {
	class GraphicsFactory;
	class CommandBuffer;
	class Surface;
	class RenderTarget;
	class PipelineObject;
	class Texture;
	class Buffer;
}

namespace clv::plt {
	class Window;
}

namespace blb::rnd {
	class Sprite;
	class Mesh;
    class SkeletalMesh;
}

namespace blb::rnd {
	class Renderer3D {
		//TYPES
	public:
		struct ComposedCameraData {
			clv::gfx::Viewport viewport;
			CameraRenderData bufferData;
			std::shared_ptr<clv::gfx::RenderTarget> target;
		};

	private:
		struct SceneData {
			//VARIABLES
		public:
			std::vector<ComposedCameraData> cameras;

			std::vector<std::shared_ptr<rnd::Mesh>> meshes;
            std::vector<std::shared_ptr<rnd::Mesh>> animatedMeshes;

			int32_t numDirectionalLights = 0u;
			int32_t numPointLights = 0u;

			LightDataArray lightDataArray;

			std::array<std::array<clv::mth::mat4f, 6>, MAX_LIGHTS> pointShadowTransformArray{};
			DirectionalShadowTransformArray directionalShadowTransformArray{};

			std::vector<std::shared_ptr<Sprite>> widgetsToRender;
			std::vector<std::shared_ptr<Sprite>> textToRender;

			//FUNCTIONS
		public:
			SceneData();

			~SceneData();
		};

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::RenderTarget> directionalShadowRenderTarget;
		std::shared_ptr<clv::gfx::RenderTarget> pointShadowRenderTarget;

		std::shared_ptr<clv::gfx::PipelineObject> directionalShadowPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> pointShadowPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> meshPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> animatedMeshPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> widgetPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> textPipelineObject;

		std::shared_ptr<clv::gfx::CommandBuffer> directionalShadowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> pointShadowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> meshCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> uiCommandBuffer;

		std::shared_ptr<clv::gfx::Texture> directionalShadowMapTexture;
		std::shared_ptr<clv::gfx::Texture> pointShadowMapTexture;

		std::shared_ptr<clv::gfx::Buffer> viewBuffer;
		std::shared_ptr<clv::gfx::Buffer> viewPosition;

		std::shared_ptr<clv::gfx::Buffer> widgetVB;
		std::shared_ptr<clv::gfx::Buffer> textVB;
		std::shared_ptr<clv::gfx::Buffer> uiIndexBuffer;

		std::shared_ptr<clv::gfx::Buffer> lightArrayBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightDepthBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightNumBuffer;

		std::shared_ptr<clv::gfx::Buffer> directionalShadowTransformBuffer;
		std::shared_ptr<clv::gfx::Buffer> directionalShadowTransformArrayBuffer;
		std::shared_ptr<clv::gfx::Buffer> pointShadowTransformBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightIndexBuffer;
		std::shared_ptr<clv::gfx::Buffer> shadowLightPosBuffer;

		clv::gfx::VertexLayout uiVbLayout;

		SceneData scene;

		//FUNCTIONS
	public:
		Renderer3D(clv::plt::Window& window);
		//TODO: ctors

		void begin();

		void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh);
        void submitAnimatedMesh(const std::shared_ptr<rnd::Mesh>& mesh);
		void submitLight(const DirectionalLight& light);
		void submitLight(const PointLight& light);
		void submitCamera(const ComposedCameraData& camera);

		void submitWidget(const std::shared_ptr<Sprite>& widget);
		void submitText(const std::shared_ptr<Sprite>& text);

		void end();
	};
}