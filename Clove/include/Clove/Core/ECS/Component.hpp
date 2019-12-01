#pragma once

#include "Clove/Core/ECS/ECSTypes.hpp"
#include "Clove/Core/Utils/HashString.hpp"

namespace clv::ecs{
	class ComponentInterface{
		//FUNCTIONS
	public:
		virtual ~ComponentInterface() = default;

		virtual ComponentID getID() const = 0;
	};

	template<typename DerivedClassType>
	class Component : public ComponentInterface{
		//VARIABLES
	public:
		EntityID entityID = INVALID_ENTITY_ID; //TODO: Make private

		//FUNCTIONS
	public:
		Component() = default;

		Component(const Component& other);
		Component(Component&& other) noexcept;

		Component& operator=(const Component& other);
		Component& operator=(Component&& other) noexcept;
		
		virtual ~Component();

		static ComponentID id();

		virtual ComponentID getID() const override;
	};
}

#include "Component.inl"