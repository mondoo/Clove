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

			const auto& position = transform->getPosition();

			light->lightData.intensity.position = position;
			light->lightData.shadowTransforms[0] = math::lookAt(position, position + math::Vector3f{ 1.0, 0.0, 0.0}, math::Vector3f{0.0,-1.0, 0.0});
			light->lightData.shadowTransforms[1] = math::lookAt(position, position + math::Vector3f{-1.0, 0.0, 0.0}, math::Vector3f{0.0,-1.0, 0.0});
			light->lightData.shadowTransforms[2] = math::lookAt(position, position + math::Vector3f{ 0.0, 1.0, 0.0}, math::Vector3f{0.0, 0.0, 1.0});
			light->lightData.shadowTransforms[3] = math::lookAt(position, position + math::Vector3f{ 0.0,-1.0, 0.0}, math::Vector3f{0.0, 0.0,-1.0});
			light->lightData.shadowTransforms[4] = math::lookAt(position, position + math::Vector3f{ 0.0, 0.0, 1.0}, math::Vector3f{0.0,-1.0, 0.0});
			light->lightData.shadowTransforms[5] = math::lookAt(position, position + math::Vector3f{ 0.0, 0.0,-1.0}, math::Vector3f{0.0,-1.0, 0.0});

			gfx::Renderer::submitPointLight(light->lightData);
		}
	}
}