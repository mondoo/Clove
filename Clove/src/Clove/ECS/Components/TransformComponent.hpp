#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	class TransformComponent : public Component{
		//VARIABLES
	private:
		unsigned int componentID = ++nextID;
		
		math::Vector3f position = { 0.0f };
		math::Vector3f rotation = { 0.0f };
		math::Vector3f scale	= { 0.0f };

		//FUNCTIONS
	public:
		TransformComponent();
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) noexcept;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept;
		virtual ~TransformComponent();
	};
}
