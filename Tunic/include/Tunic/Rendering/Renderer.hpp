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
	class Buffer;
}

namespace clv::plt{
	class Window;
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
		std::shared_ptr<clv::gfx::RenderTarget> shadowRenderTarget;

		std::shared_ptr<clv::gfx::PipelineObject> shadowPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> meshPipelineObject;

		std::shared_ptr<clv::gfx::CommandBuffer> shadowCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> meshCommandBuffer;

		std::shared_ptr<clv::gfx::Texture> shadowMapTexture;

		std::shared_ptr<clv::gfx::Buffer> viewBuffer;
		std::shared_ptr<clv::gfx::Buffer> viewPosition;

		//TODO: These need to be renamed
		std::shared_ptr<clv::gfx::Buffer> lightInfoBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightDepthBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightNumBuffer;

		std::shared_ptr<clv::gfx::Buffer> shadowInfoBuffer;
		std::shared_ptr<clv::gfx::Buffer> lightIndexBuffer;
		std::shared_ptr<clv::gfx::Buffer> shadowLightPosBuffer;

		SceneData scene;

		//FUNCTIONS
	public:
		Renderer(clv::plt::Window& window);
		//TODO: ctors

		void begin();

		void submitMesh(const std::shared_ptr<rnd::Mesh>& mesh);
		void submitLight(const PointLightData& light);
		void submitCamera(const ComposedCameraData& camera);

		void end();
	};
}