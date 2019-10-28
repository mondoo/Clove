#include "Manager.hpp"

#include "Core/ECS/2D/Systems/RenderSystem.hpp"
#include "Core/ECS/3D/Systems/RenderSystem.hpp"
#include "Core/ECS/2D/Systems/TransformSystem.hpp"
#include "Core/ECS/3D/Systems/TransformSystem.hpp"
#include "Core/ECS/3D/Systems/LightSystem.hpp"
#include "Core/ECS/3D/Systems/CameraSystem.hpp"
#include "Core/ECS/Audio/Systems/AudioSystem.hpp"
#include "Core/ECS/UI/Systems/TextSystem.hpp"

namespace clv::ecs{
	EntityID Manager::nextID = 0;

	Manager::Manager(){
		//Order is somewhat important
		systems[0] = std::make_unique<_2D::TransformSystem>();
		systems[1] = std::make_unique<_2D::RenderSystem>();

		systems[2] = std::make_unique<_3D::TransformSystem>();
		systems[3] = std::make_unique<_3D::RenderSystem>();
		systems[4] = std::make_unique<_3D::LightSystem>();
		systems[5] = std::make_unique<_3D::CameraSystem>();

		systems[6] = std::make_unique<aud::AudioSystem>();

		systems[7] = std::make_unique<ui::TextSystem>();
	}

	Manager::~Manager() = default;

	void Manager::update(utl::DeltaTime deltaTime){
		for(const auto& system : systems){
			system->update(deltaTime);
		}
	}

	Entity Manager::createEntity(){
		EntityID ID = ++nextID;

		Entity entity{ ID };
		bindEntity(entity);

		return entity;
	}

	void Manager::destroyEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return;
		}

		for(const auto& system : systems){
			system->onEntityDestroyed(ID);
		}
		components.erase(ID);
	}

	Entity Manager::getEntity(EntityID ID){
		if(const auto foundEnt = components.find(ID); foundEnt != components.end()){
			Entity entity{ ID };
			bindEntity(entity);

			return entity;
		}
		return {};
	}

	void Manager::onEntityCreateComponent(EntityID entityID, ComponentID componentID, std::unique_ptr<Component> component){
		components[entityID][componentID] = std::move(component);

		for(auto& system : systems){
			system->onEntityComponentAdded(entityID, components[entityID]);
		}
	}

	Component* Manager::getComponentForEntity(EntityID entityID, ComponentID componentID){
		return components[entityID][componentID].get();
	}

	bool Manager::isEntityValid(EntityID entityID){
		const auto it = components.find(entityID);
		return it != components.end();
	}
	
	void Manager::bindEntity(Entity& entity){
		entity.onComponentCreated.bind(&Manager::onEntityCreateComponent, this);
		entity.onComponentRequestedDelegate.bind(&Manager::getComponentForEntity, this);
		entity.isEntityIdValidDelegate.bind(&Manager::isEntityValid, this);
	}
}