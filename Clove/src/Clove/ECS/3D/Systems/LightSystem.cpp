#include "clvpch.hpp"
#include "LightSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs{
	LightSystem::LightSystem() = default;

	LightSystem::LightSystem(LightSystem&& other) noexcept = default;

	LightSystem& LightSystem::operator=(LightSystem&& other) noexcept = default;

	LightSystem::~LightSystem() = default;

	void LightSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			LightComponent* light = std::get<LightComponent*>(componentTuple);

			light->lightData.position = transform->getPosition();

			gfx::Renderer::submitLight(light->lightData);
		}
	}
}