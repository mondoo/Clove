#include "clvpch.hpp"
#include "Manager.hpp"

#include "Clove/ECS/2D/Systems/RenderSystem.hpp"
#include "Clove/ECS/3D/Systems/RenderSystem.hpp"
#include "Clove/ECS/2D/Systems/TransformSystem.hpp"
#include "Clove/ECS/3D/Systems/TransformSystem.hpp"
#include "Clove/ECS/3D/Systems/LightSystem.hpp"
#include "Clove/ECS/3D/Systems/CameraSystem.hpp"
#include "Clove/ECS/Audio/Systems/AudioSystem.hpp"

namespace clv::ecs{
	EntityID Manager::nextID = 0;

	Manager::Manager(){
		systems.reserve(6);
		systems.emplace_back(std::make_unique<d2::RenderSystem>());
		systems.emplace_back(std::make_unique<d3::RenderSystem>());
		systems.emplace_back(std::make_unique<d2::TransformSystem>());
		systems.emplace_back(std::make_unique<d3::TransformSystem>());
		systems.emplace_back(std::make_unique<d3::LightSystem>());
		systems.emplace_back(std::make_unique<d3::CameraSystem>());
		systems.emplace_back(std::make_unique<aud::AudioSystem>());
	}

	Manager::~Manager() = default;

	void Manager::update(utl::DeltaTime deltaTime){
		for(const auto& system : systems){
			system->update(deltaTime);
		}
	}

	void Manager::destroyEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return;
		}

		components.erase(ID);
		for(const auto& system : systems){
			system->onEntityDestroyed(ID);
		}
	}

	Entity Manager::getEntity(EntityID ID){
		if(const auto foundEnt = components.find(ID); foundEnt != components.end()){
			Entity entity{ ID };
			entity.onComponentRequestedDelegate.bind(&Manager::getComponentForEntity, this);
			entity.isEntityIdValidDelegate.bind(&Manager::isEntityValid, this);

			return entity;
		}
		return {};
	}

	Component* Manager::getComponentForEntity(EntityID entityID, ComponentID componentID){
		return components[entityID][componentID].get();
	}

	bool Manager::isEntityValid(EntityID entityID){
		const auto it = components.find(entityID);
		return it != components.end();
	}
}