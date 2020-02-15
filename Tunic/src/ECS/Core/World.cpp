#include "Tunic/ECS/Core/World.hpp"

#include "Tunic/ECS/Core/Entity.hpp"
#include "Tunic/ECS/2D/Systems/RenderSystem.hpp"
#include "Tunic/ECS/2D/Systems/PhysicsSystem.hpp"
#include "Tunic/ECS/3D/Systems/RenderSystem.hpp"
#include "Tunic/ECS/3D/Systems/PhysicsSystem.hpp"
#include "Tunic/ECS/Audio/Systems/AudioSystem.hpp"

using namespace clv;

namespace tnc::ecs{
	EntityID World::nextID = 1;
	std::queue<EntityID> World::recycledIDs;

	World::World(){
		componentManager.componentAddedDelegate.bind(&World::onComponentAdded, this);
		componentManager.componentRemovedDelegate.bind(&World::onComponentRemoved, this);
	}

	World::~World() = default;

	void World::update(utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		std::for_each(systems.begin(), systems.end(), [this](const std::unique_ptr<System>& system){
			system->preUpdate(*this);
		});

		std::for_each(systems.begin(), systems.end(), [this, deltaTime](const std::unique_ptr<System>& system){
			system->update(*this, deltaTime);
		});

		std::for_each(systems.begin(), systems.end(), [this](const std::unique_ptr<System>& system){
			system->postUpdate(*this);
		});
	}

	Entity World::createEntity(){
		EntityID ID = INVALID_ENTITY_ID;

		if(!recycledIDs.empty()){
			ID = recycledIDs.front();
			recycledIDs.pop();
		} else{
			ID = nextID++;
		}

		activeIDs.push_back(ID);

		return { ID, this };
	}

	Entity World::cloneEntitiesComponents(EntityID ID){
		Entity clonedEntity = createEntity();

		componentManager.cloneEntitiesComponents(ID, clonedEntity.getID());

		return clonedEntity;
	}

	Entity World::getEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return {};
		} else{
			return { ID, this };
		}
	}

	void World::destroyEntity(EntityID ID){
		auto foundIDIter = std::find(activeIDs.begin(), activeIDs.end(), ID);

		if(ID == INVALID_ENTITY_ID || foundIDIter == activeIDs.end()){
			return;
		}
		componentManager.onEntityDestroyed(ID);

		recycledIDs.push(ID);
		activeIDs.erase(foundIDIter);
	}

	void World::onComponentAdded(ComponentInterface* component){
		std::for_each(systems.begin(), systems.end(), [component](const std::unique_ptr<System>& system){
			system->onComponentCreated(component);
		});
	}

	void World::onComponentRemoved(ComponentInterface* component){
		std::for_each(systems.begin(), systems.end(), [component](const std::unique_ptr<System>& system){
			system->onComponentDestroyed(component);
		});
	}
}
