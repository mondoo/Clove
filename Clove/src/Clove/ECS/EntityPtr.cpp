#include "clvpch.hpp"
#include "EntityPtr.hpp"

namespace clv::ecs{
	EntityPtr::EntityPtr() = default;
	EntityPtr::EntityPtr(const EntityPtr& other) = default;

	EntityPtr::EntityPtr(EntityPtr&& other) noexcept = default;

	EntityPtr& EntityPtr::operator=(const EntityPtr& other) = default;

	EntityPtr& EntityPtr::operator=(EntityPtr&& other) noexcept = default;

	EntityPtr::~EntityPtr() = default;

	EntityPtr::EntityPtr(Manager* manager, EntityID entityID)
		: manager(manager)
		, entityID(entityID){
	}

	bool EntityPtr::isValid() const{
		if(entityID == INVALID_ENTITY_ID){
			return false;
		}

		const auto it = manager->entities.find(entityID);
		return it != manager->entities.end();
	}

	EntityID EntityPtr::getID(){
		return entityID;
	}
}