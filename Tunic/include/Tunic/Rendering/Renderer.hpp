#pragma once

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

		//FUNCTIONS
	public:
		Renderer(clv::gfx::GraphicsFactory& factory, std::shared_ptr<clv::gfx::Surface> surface);
		//TODO: ctors

		void begin();

		void submitMesh(const Mesh& mesh);
		void submitSprite(const Sprite& sprite);

		void end();
	};
}