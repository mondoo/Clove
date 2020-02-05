#pragma once

namespace clv::gfx{
	class CommandBuffer;
}

namespace tnc::rnd{
	class Sprite;
	class Mesh;
}

namespace tnc::rnd{
	class Renderer{
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::CommandBuffer> spriteCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> meshCommandBuffer;
		std::shared_ptr<clv::gfx::CommandBuffer> shadowCommandBuffer;

		//FUNCTIONS
	public:
		//TODO: ctors

		void begin();

		void submitSprite(const Sprite& sprite);
		void submitMesh(const Mesh& mesh);

		void end();
	};
}