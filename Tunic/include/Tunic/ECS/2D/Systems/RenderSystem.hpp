#pragma once

#include "Tunic/ECS/Core/System.hpp"

namespace clv::gfx{
	class CommandBuffer;
	class PipelineObject;
}

namespace tnc::rnd{
	class Mesh;
	class Sprite;
}

namespace tnc::ecs::_2D{
	class RenderSystem : public System{
		//TYPES
	private:
		struct SceneData{
			std::shared_ptr<rnd::Mesh> spriteMesh;
			std::shared_ptr<rnd::Mesh> widgetMesh;
			std::shared_ptr<rnd::Mesh> characterMesh;

			std::vector<std::shared_ptr<rnd::Sprite>> spritesToRender;
			std::vector<std::shared_ptr<rnd::Sprite>> widgetsToRender;
			std::vector<std::shared_ptr<rnd::Sprite>> charactersToRender;

			std::shared_ptr<clv::gfx::PipelineObject> spritePipelineObject;
			std::shared_ptr<clv::gfx::PipelineObject> charPipelineObject;
		};

		//VARIABLES
	private:
		SceneData sceneData;
		std::shared_ptr<clv::gfx::CommandBuffer> commandBuffer;

		//FUNCTIONS
	public:
		RenderSystem();

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void preUpdate() override;
		virtual void update(clv::utl::DeltaTime deltaTime) override;
		virtual void postUpdate() override;
	};
}
