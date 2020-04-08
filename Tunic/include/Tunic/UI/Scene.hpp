#pragma once

namespace tnc::rnd {
	class Renderer2D;
}

namespace tnc::ui {
	class Widget;
}

namespace tnc::ui {
	class Scene {
		//VARIABLES
	private:
		std::vector<std::shared_ptr<Widget>> elements;

		//FUNCTIONS
	public:
		//TODO: Ctors

		void draw(rnd::Renderer2D& renderer);
	};
}