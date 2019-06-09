#pragma once

//components hold data
//has no functionality

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Component{ //struct?
		//VARIABLES
	public:
		//Getters for these?
		EntityID entityID = INVALID_ENTITY_ID;
		//size_t poolIndex = INVALID_POOL_INDEX; //size_t ???

		//FUNCTIONS
	public:
		Component();
		Component(const Component& other) = delete;
		Component(Component&& other) noexcept;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept;
		virtual ~Component();
		
		//virtual void init() = 0; //init on construct?

	};
}