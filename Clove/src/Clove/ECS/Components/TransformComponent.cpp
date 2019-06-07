#include "clvpch.hpp"
#include "TransformComponent.hpp"

namespace clv::ecs{
	TransformComponent::TransformComponent() = default;

	TransformComponent::TransformComponent(TransformComponent&& other) noexcept = default;

	TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept = default;

	TransformComponent::~TransformComponent() = default;
}