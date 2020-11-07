#include "Bulb/UI/Image.hpp"

//#include "Bulb/Rendering/Renderables/Sprite.hpp"
//#include "Bulb/Rendering/Renderer3D.hpp"

using namespace clv;
using namespace clv::gfx;

namespace blb::ui {
	Image::Image(std::shared_ptr<GraphicsFactory> graphicsFactory) {
		//sprite = std::make_shared<rnd::Sprite>(std::move(graphicsFactory));
	}

	void Image::setPosition(clv::mth::vec2f position) {
		this->position = std::move(position);
	}

	void Image::setRotation(float rotation) {
		this->rotation = rotation;
	}

	void Image::setSize(clv::mth::vec2f size) {
		this->size = std::move(size);
	}

	const clv::mth::vec2f& Image::getPosition() const {
		return position;
	}

	float Image::getRotation() const {
		return rotation;
	}

	const clv::mth::vec2f& Image::getSize() const {
		return size;
	}

    void Image::draw(rnd::ForwardRenderer3D &renderer, const clv::mth::vec2f &drawSpace) {
        const mth::vec2f screenHalfSize{ static_cast<float>(drawSpace.x) / 2.0f, static_cast<float>(drawSpace.y) / 2.0f };

		//Move the position to origin at the top left
		const mth::vec2f pos = { position.x - screenHalfSize.x, -position.y + screenHalfSize.y };

		const mth::mat4f translation	= mth::translate(mth::mat4f{ 1.0f }, { pos, 0.0f });
		const mth::mat4f rotation		= mth::rotate(mth::mat4f{ 1.0f }, this->rotation, { 0.0f, 0.0f, 1.0f });
		const mth::mat4f scale			= mth::scale(mth::mat4f{ 1.0f }, mth::vec3f{ size, 0.0f });

		const mth::mat4f model = translation * rotation * scale;
		const mth::mat4f projection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);

		//sprite->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, projection * model, ShaderStage::Vertex);

		//renderer.submitWidget(sprite);
    }
}