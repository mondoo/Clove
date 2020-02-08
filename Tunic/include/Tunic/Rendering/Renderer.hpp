#pragma once

#include "Tunic/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/Core/GraphicsTypes.hpp>

namespace clv::gfx{
	class GraphicsFactory;
	class CommandBuffer;
	class Surface;
	class RenderTarget;
	class PipelineObject;
	class Texture;
}

namespace tnc::rnd{
	class Sprite;
	class Mesh;
}

namespace tnc::rnd{
	class Renderer{
		//TYPES
	public:
		struct ComposedCameraData{
			clv::gfx::Viewport viewport;
			CameraRenderData bufferData;
			std::shared_ptr<clv::gfx::RenderTarget> target;
		};

	private:
		struct SceneData{
			//VARIABLES
		public:
			std::vector<ComposedCameraData> cameras;

			std::vector<std::shared_ptr<rnd::Mesh>> meshes;

			PointLightShaderData currentLightInfo;
			PointShadowDepthData currentShadowDepth;
			uint32_t numLights = 0u;
			std::array<std::array<clv::mth::mat4f, 6>, MAX_LIGHTS> shadowTransforms = {};

			//FUNCTIONS
		public:
			SceneData();

			~SceneData();
		};

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Surface> surface;
		std::shared_ptr<clv::gfx::RenderTarget> shadowRenderTarget;

		std::shared_ptr<clv::gfx::PipelineObject> shadowPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> meshPipelineObject;

		std::shared_ptr<clv::gfx::PipelineObject> spritePipelineObject;

		std::shared_ptr<clv::gfx::CommandBuffer> shadowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> meshCommandBuffer;

		std::shared_ptr<clv::gfx::CommandBuffer> spriteCommandBuffer;

		std::shared_ptr<clv::gfx::Texture> shadowMapTexture;

		SceneData scene;

		//FUNCTIONS
	public:
		Renderer(clv::gfx::GraphicsFactory& factory, std::shared_ptr<clv::gfx::Surface> surface);
		//TODO: ctors

		void begin();

		void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh);
		void submitLight(const PointLightData& light);
		void submitCamera(const ComposedCameraData& camera);

		void submitSprite(const Sprite& sprite);

		void end();
	};
}