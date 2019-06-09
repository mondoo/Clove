#include "clvpch.hpp"
#include "Manager.hpp"

#include "Clove/ECS/Systems/RenderSystem.hpp"

namespace clv::ecs{
	Manager::Manager(){
		systems.resize(1);
		systems[0] = std::make_unique<RenderSystem>();

		//init all components?
	}

	Manager::~Manager() = default;

	void Manager::update(float deltaTime){
		for(const auto& system : systems){
			system->update(deltaTime);
		}
	}

	//TODO: This will be delete
	std::unique_ptr<gfx::Renderer>& Manager::getRenderer(){
		return static_cast<RenderSystem*>(systems[0].get())->renderer;
	}

	void Manager::destroyEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return;
		}

		std::unique_ptr<Entity> entity = std::move(entities[ID]);
		for(const auto& system : systems){
			system->onEntityDestroyed(*entity);
		}
		entity.reset();
	}
}