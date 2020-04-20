#pragma once

namespace blb::rnd {
	class Renderer2D;
}

namespace blb::ui {
	class Widget{
		//VARIABLES
	protected:
		clv::mth::vec2f position{ 0.0f, 0.0f };
		float rotation{ 0.0f };
		clv::mth::vec2f scale{ 1.0f, 1.0f };

		//TODO: Add alignment 
		/*clv::mth::vec2f alignment{ 0.0f, 0.0f };
		clv::mth::vec2f anchor{ 0.0f, 0.0f };*/

		//FUNCTIONS
	public:
		//TODO: Ctors

		void setPosition(clv::mth::vec2f position);
		void setRotation(float rotation);
		void setScale(clv::mth::vec2f scale);

		const clv::mth::vec2f& getPosition() const;
		float getRotation() const;
		const clv::mth::vec2f& getScale() const;

		virtual void draw(rnd::Renderer2D& renderer, const clv::mth::vec2f& drawSpace) = 0;
	};
}