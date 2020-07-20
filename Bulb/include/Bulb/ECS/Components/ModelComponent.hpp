#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Rendering/Renderables/StaticModel.hpp"

namespace blb::ecs {
	class ModelComponent : public Component<ModelComponent> {
		friend class RenderSystem;

		//VARIABLES
	private:
		rnd::StaticModel model;

		//FUNCTIONS
	public:
		ModelComponent() = delete;
		ModelComponent(rnd::StaticModel model);

		ModelComponent(const ModelComponent& other);
		ModelComponent(ModelComponent&& other) noexcept;

		ModelComponent& operator=(const ModelComponent& other);
		ModelComponent& operator=(ModelComponent&& other) noexcept;

		~ModelComponent();

		rnd::StaticModel& getModel();
	};
}