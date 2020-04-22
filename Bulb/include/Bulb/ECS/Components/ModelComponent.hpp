#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Rendering/Renderables/Model.hpp"

namespace blb::ecs {
	class ModelComponent : public Component<ModelComponent> {
		friend class RenderSystem;

		//VARIABLES
	private:
		rnd::Model model;

		//FUNCTIONS
	public:
		ModelComponent() = delete;
		ModelComponent(rnd::Model model);

		ModelComponent(const ModelComponent& other);
		ModelComponent(ModelComponent&& other) noexcept;

		ModelComponent& operator=(const ModelComponent& other);
		ModelComponent& operator=(ModelComponent&& other) noexcept;

		~ModelComponent();
	};
}