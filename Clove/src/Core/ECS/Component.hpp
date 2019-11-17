#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/Utils/HashString.hpp"

namespace clv::ecs{
	class ComponentInterface{
		//FUNCTIONS
	public:
		virtual ComponentID getID() const = 0;

		virtual ~ComponentInterface() = default;
	};

	template<typename DerivedClassType>
	class Component : public ComponentInterface{
		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(DerivedClassType);

		EntityID entityID = INVALID_ENTITY_ID; //TODO: Make private

		//FUNCTIONS
	public:
		Component() = default;

		Component(const Component& other);
		Component(Component&& other) noexcept;

		Component& operator=(const Component& other);
		Component& operator=(Component&& other) noexcept;
		
		virtual ~Component();

		virtual ComponentID getID() const;
	};
}

#include "Component.inl"