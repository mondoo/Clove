#include "Tunic/UI/Image.hpp"

#include "Tunic/Rendering/Renderables/Sprite.hpp"
#include "Tunic/Rendering/Renderer2D.hpp"

//Temp?
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Platform/Core/Window.hpp"
#include "Tunic/Application.hpp"

/*
I think I need something that wraps the renderer, something that does the extra bits.
	- The renderer should only really take the data it has recieved and convert that into draw commands (which it kind of does)
	- There needs to be something like a 'render pipeline'
		- The RenderSystems kind of did this but they should just convert the data anbd pass it to the renderer

*/

using namespace clv;
using namespace clv::gfx;

namespace tnc::ui {
	void Image::draw(rnd::Renderer2D& renderer) {
		//TODO: Nothing is calling Renderer2D::begin / end

		//Ported from 2D::RenderSystem
		//Screen size
		const mth::vec2i screenSize = tnc::Application::get().getMainWindow().getSize();
		const mth::vec2f screenHalfSize{ static_cast<float>(screenSize.x) / 2.0f, static_cast<float>(screenSize.y) / 2.0f };

		const float scaleFactor = 40.0f; //How big we want our 2D elements to be

		//We want our UI to translate pixel perfect, so we create a projection that's the size of the screen and then scale the model
		const mth::mat4f uiProjection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);
		const mth::mat4f uiScale = mth::scale(mth::mat4f{ 1.0f }, mth::vec3f{ scaleFactor });

		//Rendering
		const mth::vec2f scaledScreenSize = { (screenHalfSize.x / scale.x), (screenHalfSize.y / scale.y) };

		/*mth::vec2f offset{};
		const mth::vec2f anchor = transform->getAnchor();*/

		/*if(ui::TransformComponent* parent = transform->getParent()) {
			const mth::vec2f parentScale = parent->getScale();
			offset.x = anchor.x * parentScale.x * scaleFactor;
			offset.y = anchor.y * parentScale.x * scaleFactor;
		} else {
			offset.x = anchor.x * screenSize.x;
			offset.y = anchor.y * screenSize.y;
		}*/

		const mth::mat4f modelData = {};/*mth::translate(transform->getWorldTransformMatrix(), mth::vec3f{ -scaledScreenSize.x + offset.x, scaledScreenSize.y - offset.y, 0.0f });*/

		sprite->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, uiProjection * modelData * uiScale, ShaderStage::Vertex);

		renderer.submitWidget(sprite);
	}
}