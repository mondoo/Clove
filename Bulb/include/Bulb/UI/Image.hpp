#pragma once

#include "Bulb/UI/DrawableElement.hpp"

namespace clv::gfx {
	class GraphicsFactory;
}

namespace blb::rnd {
	class Sprite;
}

namespace blb::ui {
	class Image : public DrawableElement {
		//VARIABLES
	//private:
	public:
		std::shared_ptr<rnd::Sprite> sprite;

		clv::mth::vec2f position{ 0.0f, 0.0f };
		float rotation{ 0.0f };
		clv::mth::vec2f scale{ 1.0f, 1.0f };

		//FUNCTIONS
	public:
		Image(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		//TODO: Ctors

		//TODO: Interface for these?
		void setPosition(clv::mth::vec2f position);
		void setRotation(float rotation);
		void setScale(clv::mth::vec2f scale);

		const clv::mth::vec2f& getPosition() const;
		float getRotation() const;
		const clv::mth::vec2f& getScale() const;

		void draw(rnd::Renderer3D& renderer, const clv::mth::vec2f& drawSpace) override;
	};
}