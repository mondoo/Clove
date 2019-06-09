#pragma once

//systems provide functionality
//holds no data

#include "Clove/ECS/Entity.hpp"

namespace clv::ecs{
	class Component;

	class BaseSystem{ //Do we want / need this? (not going too complicated)
	protected:
		//some sort of entity manager?

		virtual ~BaseSystem() = default;
		virtual void onEntityCreated(const Entity& entity) = 0;
		virtual void onEntityDestroyed(const Entity& entity) = 0;
		virtual void update(float deltaTime) = 0;
	};

	template<typename... Comps>
	class System : public BaseSystem{
		//care about x entities that have specific components (render needs trans + render)

		using EntityIDToIndexMap = std::unordered_map<EntityID, size_t, std::hash<EntityID>, std::equal_to<EntityID>/*, PooledAllocator*/>;
		EntityIDToIndexMap entityIdToIndexMap; //So we can pair an entity id to an index we have in oru vec

		//VARIABLES
	protected:
		//TODO: Look up what this add pointer thing really does! (why do i need it?) - I'm fairly certain it'll make the types passed in as ptr types instead of value types
		using CompTuple = std::tuple<std::add_pointer_t<Comps>...>; //makes a tuple of all components passed in
		std::vector<CompTuple/*, PooledAllocator*/> components; //TODO: using default allocator but we'd want a custom one to allign memory for iterating
	
	private:

		//FUNCTIONS
	public:
		//virtual void init() = 0; //init construct?

		//TODO: defining here for now
		virtual void onEntityCreated(const Entity& entity) override final{
			//loop through all comps on entity and see if any match our tuple
			CompTuple comptuple;
			size_t matchingComps = 0;
			for(const auto&[id, comp] : entity.getComponents()){
				if(proccessEntityComponent<0, Comps...>(id, comp, comptuple)){
					++matchingComps;
					if(matchingComps == sizeof...(Comps)){ //if the size is equal to the size of our variadic template...
						//THen we're done!
						components.emplace_back(comptuple);
						entityIdToIndexMap[entity.getID()] = components.size() - 1;
						break;
					}
				}
			}
		}

		virtual void onEntityDestroyed(const Entity& entity) override final{
			const auto findIt = entityIdToIndexMap.find(entity.getID());
			if(findIt != entityIdToIndexMap.end()){ //we have it!
				//move the back element to override this one (constant time removal if sorting isn't an issue!)
				components[findIt->second] = std::move(components.back());
				components.pop_back();

				//However now we need to find out which entity the moved component belong to
				//so we can update the id to index map. since all components in a single tuple are owned by the
				//same entity we can jusr grab the firest one and query it
				const auto* movedComponent = std::get<0>(components[findIt->second]);

				//the moved component from this point is basically what ever the back element was
				//the one we just moved into the spot

				//find the entry for the moved id and update it with the new index
				auto movedTupleIt = entityIdToIndexMap.find(movedComponent->entityID);

				//now if we have the data (the key and val) or the one we just moved.
				//however the val (index) is incorrect because we moved it earlier

				CLV_ASSERT(movedTupleIt == entityIdToIndexMap.end(), "Oh no!"); //should be impossible but we should check

				//so here we just give it the index of the one we removed
				movedTupleIt->second = findIt->second; 
			}
		}

	private:
		//recursively looks through component types
		template<size_t INDEX, typename CompType, typename... CompArgs>
		bool proccessEntityComponent(ComponentID id, Component* comp, CompTuple& tupleToFill){
			if(CompType::ID == id){ //TODO: need a statically accessable ID for components
				std::get<INDEX>(tupleToFill) == static_cast<CompType*>(comp);
				return true;
			} else{
				return proccessEntityComponent<INDEX + 1, CompArgs...>(id, comp, tupleToFill);
			}
		}
		//Termination of recusrive functions (when we run out of comp types we will reach this function)
		template<size_t INDEX>
		bool proccessEntityComponent(ComponentID id, Component* comp, CompTuple& tupleToFill){
			return false;
		}
	};
}
