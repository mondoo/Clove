#pragma once

#include "Tunic/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

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

namespace tnc::rnd {
	class Sprite;
	class Mesh;
}

namespace tnc::rnd {
	class Renderer {
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

			int32_t numDirectionalLights = 0u;
			int32_t numPointLights = 0u;

			LightDataArray lightDataArray;

			std::array<std::array<clv::mth::mat4f, 6>, MAX_LIGHTS> pointShadowTransformArray{};
			DirectionalShadowTransformArray directionalShadowTransformArray{};

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

		std::shared_ptr<clv::gfx::CommandBuffer> directionalShadowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> pointShadowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> meshCommandBuffer;

		std::shared_ptr<clv::gfx::Texture> directionalShadowMapTexture;
		std::shared_ptr<clv::gfx::Texture> pointShadowMapTexture;

		std::shared_ptr<clv::gfx::Buffer> viewBuffer;
		std::shared_ptr<clv::gfx::Buffer> viewPosition;

		std::shared_ptr<clv::gfx::Buffer> lightArrayBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightDepthBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightNumBuffer;

		std::shared_ptr<clv::gfx::Buffer> directionalShadowTransformBuffer;
		std::shared_ptr<clv::gfx::Buffer> directionalShadowTransformArrayBuffer;
		std::shared_ptr<clv::gfx::Buffer> pointShadowTransformBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightIndexBuffer;
		std::shared_ptr<clv::gfx::Buffer> shadowLightPosBuffer;

		SceneData scene;

		//FUNCTIONS
	public:
		Renderer(clv::plt::Window& window);
		//TODO: ctors

		void begin();

		void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh);
		void submitLight(const DirectionalLight& light);
		void submitLight(const PointLight& light);
		void submitCamera(const ComposedCameraData& camera);

		void end();
	};
}