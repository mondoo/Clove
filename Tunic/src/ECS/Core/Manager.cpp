#include "Tunic/ECS/Core/Manager.hpp"

#include "Tunic/ECS/Core/Entity.hpp"
#include "Tunic/ECS/2D/Systems/RenderSystem.hpp"
#include "Tunic/ECS/2D/Systems/PhysicsSystem.hpp"
#include "Tunic/ECS/3D/Systems/RenderSystem.hpp"
#include "Tunic/ECS/3D/Systems/PhysicsSystem.hpp"
#include "Tunic/ECS/Audio/Systems/AudioSystem.hpp"

using namespace clv;

namespace tnc::ecs{
	EntityID Manager::nextID = 1;
	std::queue<EntityID> Manager::recycledIDs;

	Manager::Manager(){
		componentManager.componentAddedDelegate.bind(&Manager::onComponentAdded, this);
		componentManager.componentRemovedDelegate.bind(&Manager::onComponentRemoved, this);
	}

	Manager::~Manager() = default;

	void Manager::update(utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		std::for_each(systems.begin(), systems.end(), [](const std::unique_ptr<System>& system){
			system->preUpdate();
		});

		std::for_each(systems.begin(), systems.end(), [deltaTime](const std::unique_ptr<System>& system){
			system->update(deltaTime);
		});

		std::for_each(systems.begin(), systems.end(), [](const std::unique_ptr<System>& system){
			system->postUpdate();
		});
	}

	Entity Manager::createEntity(){
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

	Entity Manager::cloneEntitiesComponents(EntityID ID){
		Entity clonedEntity = createEntity();

		componentManager.cloneEntitiesComponents(ID, clonedEntity.getID());

		return clonedEntity;
	}

	Entity Manager::getEntity(EntityID ID){
		if(ID == INVALID_ENTITY_ID){
			return {};
		} else{
			return { ID, this };
		}
	}

	void Manager::destroyEntity(EntityID ID){
		auto foundIDIter = std::find(activeIDs.begin(), activeIDs.end(), ID);

		if(ID == INVALID_ENTITY_ID || foundIDIter == activeIDs.end()){
			return;
		}
		componentManager.onEntityDestroyed(ID);

		recycledIDs.push(ID);
		activeIDs.erase(foundIDIter);
	}

	void Manager::onComponentAdded(ComponentInterface* component){
		std::for_each(systems.begin(), systems.end(), [component](const std::unique_ptr<System>& system){
			system->onComponentCreated(component);
		});
	}

	void Manager::onComponentRemoved(ComponentInterface* component){
		std::for_each(systems.begin(), systems.end(), [component](const std::unique_ptr<System>& system){
			system->onComponentDestroyed(component);
		});
	}
}
