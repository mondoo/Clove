#include "Clove/ECS/World.hpp"

#include "Clove/ECS/Entity.hpp"
#include "Clove/ECS/System.hpp"

namespace garlic::clove {
    EntityID World::nextID = 1;

    World::World()
        : componentManager(&ecsEventDispatcher) {
    }

    World::~World() {
        destroyAllEntites();
    }

    void World::update(DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        if(pendingDestroyIDs.size() > 0) {
            CLV_PROFILE_SCOPE("Destroying pendining entities");

            for(EntityID id : pendingDestroyIDs) {
                componentManager.onEntityDestroyed(id);
            }

            auto const removeIter = std::remove_if(activeIDs.begin(), activeIDs.end(), [this](EntityID id) {
                return pendingDestroyIDs.find(id) != pendingDestroyIDs.end();
            });
            activeIDs.erase(removeIter, activeIDs.end());

            pendingDestroyIDs.clear();
        }

        for(auto const &system : systems) {
            system->preUpdate(*this);
        }

        for(auto const &system : systems) {
            system->update(*this, deltaTime);
        }

        for(auto const &system : systems) {
            system->postUpdate(*this);
        }
    }

    Entity World::createEntity() {
        EntityID ID = nextID++;

        activeIDs.push_back(ID);

        return { ID, this };
    }

    Entity World::cloneEntitiesComponents(EntityID ID) {
        Entity clonedEntity = createEntity();

        componentManager.cloneEntitiesComponents(ID, clonedEntity.getID());

        return clonedEntity;
    }

    bool World::isEntityValid(EntityID ID) {
        if(ID != INVALID_ENTITY_ID) {
            return std::find(activeIDs.begin(), activeIDs.end(), ID) != activeIDs.end();
        } else {
            return false;
        }
    }

    Entity World::getEntity(EntityID ID) {
        if(isEntityValid(ID)) {
            return { ID, this };
        } else {
            return {};
        }
    }

    std::vector<Entity> World::getActiveEntities() {
        std::vector<Entity> entities;
        entities.reserve(activeIDs.size());
        for(EntityID id : activeIDs) {
            entities.emplace_back(id, this);
        }
        return entities;
    }

    void World::destroyEntity(EntityID ID) {
        auto foundIDIter = std::find(activeIDs.begin(), activeIDs.end(), ID);

        if(ID == INVALID_ENTITY_ID || foundIDIter == activeIDs.end()) {
            return;
        }

        pendingDestroyIDs.emplace(ID);
    }

    void World::destroyAllEntites() {
        for(EntityID id : activeIDs) {
            componentManager.onEntityDestroyed(id);
        }
        activeIDs.clear();
    }
}
