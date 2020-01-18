#include "Tunic/Rendering/Renderables/Sprite.hpp"

#include "Tunic/Application.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::rnd{
	Sprite::Sprite()
		: materialInstance(std::make_shared<Material>()){
	}

	Sprite::Sprite(const std::string& pathToTexture)
		: materialInstance(std::make_shared<Material>()){
		materialInstance.setAlbedoTexture(Application::get().getGraphicsFactory().createTexture({}, pathToTexture));
	}

	Sprite::Sprite(const std::shared_ptr<clv::gfx::Texture>& texture)
		: materialInstance(std::make_shared<Material>()){
		materialInstance.setAlbedoTexture(texture);
	}

	Sprite::Sprite(const std::shared_ptr<Material>& material)
		: materialInstance(material){
	}

	Sprite::Sprite(const Sprite& other) = default;

	Sprite::Sprite(Sprite&& other) noexcept = default;

	Sprite& Sprite::operator=(const Sprite& other) = default;

	Sprite& Sprite::operator=(Sprite&& other) noexcept = default;

	Sprite::~Sprite() = default;

	MaterialInstance& Sprite::getMaterialInstance(){
		return materialInstance;
	}
}