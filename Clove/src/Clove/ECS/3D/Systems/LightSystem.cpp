#include "clvpch.hpp"
#include "LightSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs::d3{
	LightSystem::LightSystem() = default;

	LightSystem::LightSystem(LightSystem&& other) noexcept = default;

	LightSystem& LightSystem::operator=(LightSystem&& other) noexcept = default;

	LightSystem::~LightSystem() = default;

	void LightSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			LightComponent* light = std::get<LightComponent*>(componentTuple);

			light->lightData.position = transform->getPosition();

			gfx::Renderer::submitPointLight(light->lightData);
		}
	}
}