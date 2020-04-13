#include "Tunic/ECS/Components/ModelComponent.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::ecs {
	ModelComponent::ModelComponent(rnd::Model model)
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
}