#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Rendering/Renderables/StaticModel.hpp"

namespace blb::ecs {
	class StaticModelComponent : public Component<StaticModelComponent> {
		friend class RenderSystem;

		//VARIABLES
	private:
		rnd::StaticModel model;

		//FUNCTIONS
	public:
		StaticModelComponent() = delete;
		StaticModelComponent(rnd::StaticModel model);

		StaticModelComponent(const StaticModelComponent& other);
		StaticModelComponent(StaticModelComponent&& other) noexcept;

		StaticModelComponent& operator=(const StaticModelComponent& other);
		StaticModelComponent& operator=(StaticModelComponent&& other) noexcept;

		~StaticModelComponent();

		rnd::StaticModel& getModel();
	};
}