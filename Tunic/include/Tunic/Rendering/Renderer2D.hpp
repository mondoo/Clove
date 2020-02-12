#pragma once

#include <Clove/Graphics/Core/VertexLayout.hpp>

namespace clv::gfx{
	class Buffer;
	class CommandBuffer;
	class PipelineObject;
}

namespace clv::plt{
	class Window;
}

namespace tnc::rnd{
	class Sprite;
}

namespace tnc::rnd{
	class Renderer2D{
		//TYPES
	private:
		struct SceneData{
			std::vector<std::shared_ptr<Sprite>> spritesToRender;
			std::vector<std::shared_ptr<Sprite>> widgetsToRender;
			std::vector<std::shared_ptr<Sprite>> textToRender;
		};

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Buffer> indexBuffer;

		clv::gfx::VertexLayout vbLayout;

		//TOOD: Do we need 3 vbs?
		std::shared_ptr<clv::gfx::Buffer> spriteVB;
		std::shared_ptr<clv::gfx::Buffer> widgetVB;
		std::shared_ptr<clv::gfx::Buffer> textVB;

		std::shared_ptr<clv::gfx::PipelineObject> defaultPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> textPipelineObject;

		std::shared_ptr<clv::gfx::CommandBuffer> commandBuffer;
		
		SceneData scene;

		//FUNCTIONS
	public:
		Renderer2D(clv::plt::Window& window);
		//TODO: Other ctors

		void begin();

		void sumbitSprite(const std::shared_ptr<Sprite>& sprite);
		void submitWidget(const std::shared_ptr<Sprite>& widget);
		void submitText(const std::shared_ptr<Sprite>& text);

		void end();
	};
}