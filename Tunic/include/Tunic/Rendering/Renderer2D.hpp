#pragma once

#include <Clove/Graphics/VertexLayout.hpp>

namespace clv::gfx {
	class Buffer;
	class CommandBuffer;
	class PipelineObject;
	class RenderTarget;
}

namespace clv::plt {
	class Window;
}

namespace tnc::rnd {
	class Sprite;
}

namespace tnc::rnd {
	class Renderer2D {
		//TYPES
	private:
		struct SceneData {
			std::vector<std::shared_ptr<Sprite>> spritesToRender;
			std::vector<std::shared_ptr<Sprite>> widgetsToRender;
			std::vector<std::shared_ptr<Sprite>> textToRender;
		};

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::RenderTarget> renderTarget;
		clv::mth::vec2i screenSize;

		std::shared_ptr<clv::gfx::Buffer> indexBuffer;

		clv::gfx::VertexLayout vbLayout;

		//TOOD: Do we need 2 vbs?
		std::shared_ptr<clv::gfx::Buffer> widgetVB;
		std::shared_ptr<clv::gfx::Buffer> textVB;

		std::shared_ptr<clv::gfx::PipelineObject> defaultPipelineObject;
		std::shared_ptr<clv::gfx::PipelineObject> textPipelineObject;

		std::shared_ptr<clv::gfx::CommandBuffer> commandBuffer;

		SceneData scene;

		//FUNCTIONS
	public:
		Renderer2D() = delete;
		Renderer2D(clv::plt::Window& window);
		
		Renderer2D(const Renderer2D& other) = delete;
		Renderer2D(Renderer2D&& other) noexcept;

		Renderer2D& operator=(const Renderer2D& other) = delete;
		Renderer2D& operator=(Renderer2D&& other) noexcept;

		~Renderer2D();

		void begin();

		void submitWidget(const std::shared_ptr<Sprite>& widget);
		void submitText(const std::shared_ptr<Sprite>& text);

		void end();
	};
}