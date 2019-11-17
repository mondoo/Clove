#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/Utils/HashString.hpp"

namespace clv::ecs{
	class IComponent{
		//FUNCTIONS
	public:
		virtual ComponentID getID() const = 0;
	};

	template<typename DerivedClassType>
	class Component : public IComponent{
		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(DerivedClassType);

		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		Component();

		Component(const Component& other) = delete;
		Component(Component&& other) noexcept;

		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept;
		
		virtual ~Component();

		//TODO: .inl
		virtual ComponentID getID() const{ return ID; }
	};
}