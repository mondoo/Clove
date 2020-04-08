#pragma once

namespace tnc::rnd {
	class Renderer2D;
}

namespace tnc::ui {
	class Widget{
		//VARIABLES
	private:
		clv::mth::vec2f position{ 0.0f, 0.0f };
		clv::mth::vec2f alignment{ 0.0f, 0.0f };
		clv::mth::vec2f anchor{ 0.0f, 0.0f };

		//FUNCTIONS
	public:
		//TODO: Ctors

		virtual void draw(rnd::Renderer2D& renderer) = 0;
	};
}