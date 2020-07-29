#include "Bulb/ECS/Components/StaticModelComponent.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"

namespace blb::ecs {
	StaticModelComponent::StaticModelComponent(rnd::StaticModel model)
		: model(std::move(model)) {
	}

	StaticModelComponent::StaticModelComponent(const StaticModelComponent& other) = default;

	StaticModelComponent::StaticModelComponent(StaticModelComponent&& other) noexcept
		: model(std::move(other.model)) {
	}

	StaticModelComponent& StaticModelComponent::operator=(const StaticModelComponent& other) = default;

	StaticModelComponent& StaticModelComponent::operator=(StaticModelComponent&& other) noexcept {
		model = other.model;
		return *this;
	}

	StaticModelComponent::~StaticModelComponent() = default;

	rnd::StaticModel& StaticModelComponent::getModel(){
		return model;
	}
}