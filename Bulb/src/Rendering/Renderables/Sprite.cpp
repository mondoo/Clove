#include "Bulb/Rendering/Renderables/Sprite.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/Texture.hpp>

using namespace clv;
using namespace clv::gfx;

namespace blb::rnd {
	Sprite::Sprite(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
		: materialInstance(std::make_shared<Material>(std::move(graphicsFactory))) {
	}

	Sprite::Sprite(const std::string& pathToTexture, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
		: materialInstance(std::make_shared<Material>(graphicsFactory)) {
		materialInstance.setAlbedoTexture(graphicsFactory->createTexture({}, pathToTexture));
	}

	Sprite::Sprite(const std::shared_ptr<clv::gfx::Texture>& texture, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
		: materialInstance(std::make_shared<Material>(std::move(graphicsFactory))) {
		materialInstance.setAlbedoTexture(texture);
	}

	Sprite::Sprite(const std::shared_ptr<Material>& material)
		: materialInstance(material) {
	}

	Sprite::Sprite(const Sprite& other) = default;

	Sprite::Sprite(Sprite&& other) noexcept = default;

	Sprite& Sprite::operator=(const Sprite& other) = default;

	Sprite& Sprite::operator=(Sprite&& other) noexcept = default;

	Sprite::~Sprite() = default;

	MaterialInstance& Sprite::getMaterialInstance() {
		return materialInstance;
	}
}