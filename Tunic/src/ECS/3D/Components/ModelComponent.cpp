#include "Tunic/ECS/3D/Components/ModelComponent.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::ecs::_3D {
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