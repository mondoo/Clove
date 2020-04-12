#include "Tunic/UI/Scene.hpp"

#include "Tunic/UI/Widget.hpp"

using namespace clv;
//using namespace clv::gfx;

namespace tnc::ui {
	void Scene::draw(rnd::Renderer2D& renderer) {
		//TODO: Nothing is calling Renderer2D::begin / end

		//Ported from 2D::RenderSystem
		//Screen size
		/*
		const mth::vec2i screenSize = tnc::Application::get().getMainWindow().getSize(); //TODO: Could this be gotten from the renderer?
		const mth::vec2f screenHalfSize{ static_cast<float>(screenSize.x) / 2.0f, static_cast<float>(screenSize.y) / 2.0f };

		const float scaleFactor = 40.0f; //How big we want our 2D elements to be

		//We want our UI to translate pixel perfect, so we create a projection that's the size of the screen and then scale the model
		const mth::mat4f uiProjection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);
		const mth::mat4f uiScale = mth::scale(mth::mat4f{ 1.0f }, mth::vec3f{ scaleFactor });

		for(auto& widget : elements) {
			widget->draw(renderer);
		}
		*/
	}
}