#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/Utils/HashString.hpp"

//TODO: .inl

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

		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = default;

		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = default;
		
		virtual ~Component() = default;

		virtual ComponentID getID() const{ return ID; }
	};
}