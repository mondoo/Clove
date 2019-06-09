#include "clvpch.hpp"
#include "Manager.hpp"

#include "Clove/ECS/Systems/RenderSystem.hpp"

namespace clv::ecs{
	Manager::Manager(){
		systems[RenderSystem::ID] = std::make_unique<RenderSystem>();
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

		std::unique_ptr<Entity> entity = std::move(entities[ID]);
		for(const auto& [ID, system] : systems){
			system->onEntityDestroyed(*entity);
		}
		entity.reset();
	}
}