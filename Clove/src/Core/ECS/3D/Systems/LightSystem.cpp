#include "LightSystem.hpp"

#include "Core/ECS/Manager.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/ECS/3D/Components/LightComponent.hpp"
#include "Core/Graphics/Renderer.hpp"

namespace clv::ecs::_3D{
	LightSystem::LightSystem() = default;

	LightSystem::LightSystem(LightSystem&& other) noexcept = default;

	LightSystem& LightSystem::operator=(LightSystem&& other) noexcept = default;

	LightSystem::~LightSystem() = default;

	void LightSystem::update(utl::DeltaTime deltaTime){
		auto componentTuples = manager->getComponentSets<TransformComponent, LightComponent>();
		for(auto& tuple : componentTuples){
			TransformComponent* transform = std::get<TransformComponent*>(tuple);
			LightComponent* light = std::get<LightComponent*>(tuple);

			const auto& position = transform->getPosition();

			light->lightData.intensity.position = position;
			light->lightData.shadowTransforms[0] = light->shadowProj * math::lookAt(position, position + math::Vector3f{ 1.0, 0.0, 0.0}, math::Vector3f{0.0,-1.0, 0.0});
			light->lightData.shadowTransforms[1] = light->shadowProj * math::lookAt(position, position + math::Vector3f{-1.0, 0.0, 0.0}, math::Vector3f{0.0,-1.0, 0.0});
			light->lightData.shadowTransforms[2] = light->shadowProj * math::lookAt(position, position + math::Vector3f{ 0.0, 1.0, 0.0}, math::Vector3f{0.0, 0.0, 1.0});
			light->lightData.shadowTransforms[3] = light->shadowProj * math::lookAt(position, position + math::Vector3f{ 0.0,-1.0, 0.0}, math::Vector3f{0.0, 0.0,-1.0});
			light->lightData.shadowTransforms[4] = light->shadowProj * math::lookAt(position, position + math::Vector3f{ 0.0, 0.0, 1.0}, math::Vector3f{0.0,-1.0, 0.0});
			light->lightData.shadowTransforms[5] = light->shadowProj * math::lookAt(position, position + math::Vector3f{ 0.0, 0.0,-1.0}, math::Vector3f{0.0,-1.0, 0.0});

			gfx::Renderer::submitPointLight(light->lightData);
		}
	}
}