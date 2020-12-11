#include "Clove/ECS/World.hpp"

#include "Clove/ECS/System.hpp"

namespace garlic::clove {
    Entity World::nextEntity = 1;

    World::World()
        : componentManager(&ecsEventDispatcher) {
    }

    World::~World() {
        destroyAll();
    }

    void World::update(DeltaTime deltaTime) {
        CLOVE_PROFILE_FUNCTION();

        if(pendingDestroyEntities.size() > 0) {
            CLOVE_PROFILE_SCOPE("Destroying pendining entities");

            for(Entity entity : pendingDestroyEntities) {
                componentManager.onEntityDestroyed(entity);
            }

            auto const removeIter = std::remove_if(activeEntities.begin(), activeEntities.end(), [this](Entity entity) {
                return pendingDestroyEntities.find(entity) != pendingDestroyEntities.end();
            });
            activeEntities.erase(removeIter, activeEntities.end());

            pendingDestroyEntities.clear();
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

    Entity World::create() {
        Entity entity{ nextEntity++ };
        activeEntities.push_back(entity);

        return entity;
    }

    Entity World::clone(Entity entity) {
        Entity clonedEntity{ create() };
        componentManager.cloneEntitiesComponents(entity, clonedEntity);

        return clonedEntity;
    }

    void World::destroy(Entity entity) {
        auto foundIDIter{ std::find(activeEntities.begin(), activeEntities.end(), entity) };

        if(entity == NullEntity || foundIDIter == activeEntities.end()) {
            return;
        }

        pendingDestroyEntities.emplace(entity);
    }

    void World::destroyAll() {
        for(Entity entity : activeEntities) {
            componentManager.onEntityDestroyed(entity);
        }
        activeEntities.clear();
    }

    bool World::isValid(Entity entity) {
        if(entity != NullEntity) {
            return std::find(activeEntities.begin(), activeEntities.end(), entity) != activeEntities.end();
        } else {
            return false;
        }
    }
}
