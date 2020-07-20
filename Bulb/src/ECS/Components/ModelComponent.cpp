#include "Bulb/ECS/Components/ModelComponent.hpp"

#include "Bulb/Rendering/Renderables/Mesh.hpp"

namespace blb::ecs {
	ModelComponent::ModelComponent(rnd::StaticModel model)
		: model(std::move(model)) {
	}

	ModelComponent::ModelComponent(const ModelComponent& other) = default;

	ModelComponent::ModelComponent(ModelComponent&& other) noexcept
		: model(std::move(other.model)) {
	}

	ModelComponent& ModelComponent::operator=(const ModelComponent& other) = default;

	ModelComponent& ModelComponent::operator=(ModelComponent&& other) noexcept {
		model = other.model;
		return *this;
	}

	ModelComponent::~ModelComponent() = default;

	rnd::StaticModel& ModelComponent::getModel(){
		return model;
	}
}