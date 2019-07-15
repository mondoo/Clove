#include "clvpch.hpp"
#include "Manager.hpp"

#include "Clove/ECS/Systems/Render2DSystem.hpp"
#include "Clove/ECS/Systems/Render3DSystem.hpp"
#include "Clove/ECS/Systems/LightSystem.hpp"
#include "Clove/ECS/Systems/CameraSystem.hpp"

namespace clv::ecs{
	EntityID Manager::nextID = 0;

	Manager::Manager(){
		systems[Render2DSystem::ID] = std::make_unique<Render2DSystem>();
		systems[Render3DSystem::ID] = std::make_unique<Render3DSystem>();
		systems[LightSystem::ID] = std::make_unique<LightSystem>();
		systems[CameraSystem::ID] = std::make_unique<CameraSystem>();
	}

	Manager::~Manager() = default;

	void Manager::update(float deltaTime){
		for(const auto& [ID, system] : systems){
			system->update(deltaTime);
		}
	}

	void Manager::destroyEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return;
		}

		components.erase(ID);
		for(const auto& [sysID, system] : systems){
			system->onEntityDestroyed(ID);
		}
	}

	Entity Manager::getEntity(EntityID ID){
		if(const auto foundEnt = components.find(ID); foundEnt != components.end()){
			return { this, ID };
		}
		return {};
	}
}