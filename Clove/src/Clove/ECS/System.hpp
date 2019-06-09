#pragma once

#include "Clove/ECS/Entity.hpp"

namespace clv::ecs{
	class Component;

	class SystemBase{
		//FUNCTIONS
	public:
		virtual ~SystemBase() = default;

		virtual void onEntityCreated(const Entity& entity) = 0;
		virtual void onEntityDestroyed(const Entity& entity) = 0;
		virtual void update(float deltaTime) = 0;
	};

	template<typename... Comps>
	class System : public SystemBase{
		using ComponentTuple = std::tuple<std::add_pointer_t<Comps>...>;
		using EntityIDToIndexMap = std::unordered_map<EntityID, size_t, std::hash<EntityID>, std::equal_to<EntityID>/*, PooledAllocator*/>;

		//VARIABLES
	protected:
		std::vector<ComponentTuple/*, PooledAllocator*/> components; //TODO: using default allocator but we'd want a custom one to allign memory for iterating

	private:
		EntityIDToIndexMap entityIdToIndexMap;

		//FUNCTIONS
	public:
		//TODO: .inl
		System() = default;
		System(const System& other) = delete;
		System(System&& other) noexcept{
			components = std::move(other.components);
			entityIdToIndexMap = std::move(other.entityIdToIndexMap);
		}
		System& operator=(const System& other) = delete;
		System& operator=(System&& other) noexcept{
			components = std::move(other.components);
			entityIdToIndexMap = std::move(other.entityIdToIndexMap);

			return *this;
		}
		virtual ~System() = default;

		//TODO: .inl
		virtual void onEntityCreated(const Entity& entity) override final{
			ComponentTuple comptuple;
			size_t matchingComps = 0;
			for(const auto& [compID, comp] : entity.getComponents()){
				if(proccessEntityComponent<0, Comps...>(compID, comp.get(), comptuple)){
					++matchingComps;
					if(matchingComps == sizeof...(Comps)){
						components.emplace_back(comptuple);
						entityIdToIndexMap[entity.getID()] = components.size() - 1;
						break;
					}
				}
			}
		}

		//TODO: .inl
		virtual void onEntityDestroyed(const Entity& entity) override final{
			const auto findIt = entityIdToIndexMap.find(entity.getID());
			if(findIt != entityIdToIndexMap.end()){
				components[findIt->second] = std::move(components.back());
				components.pop_back();

				const auto& movedComponent = std::get<0>(components[findIt->second]);
				auto movedTupleIt = entityIdToIndexMap.find(movedComponent->entityID);

				CLV_ASSERT(movedTupleIt == entityIdToIndexMap.end(), "Entity ID not tracked!");
				movedTupleIt->second = findIt->second;
			}
		}

	private:
		//TODO: .inl
		template<size_t INDEX, typename CompType, typename... CompArgs>
		bool proccessEntityComponent(ComponentID compID, Component* comp, ComponentTuple& tupleToFill){
			if(CompType::ID == compID){
				std::get<INDEX>(tupleToFill) = static_cast<CompType*>(comp);
				return true;
			} else{
				return proccessEntityComponent<INDEX + 1, CompArgs...>(compID, comp, tupleToFill);
			}
		}

		//TODO: .inl
		template<size_t INDEX>
		bool proccessEntityComponent(ComponentID id, Component* comp, ComponentTuple& tupleToFill){
			return false;
		}
	};
}
