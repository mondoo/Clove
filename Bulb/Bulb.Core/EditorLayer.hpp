#pragma once

#include <Clove/Core/Layer.hpp>
#include <Clove/Core/ECS/Entity.hpp>

namespace clv::blb{
	class EditorLayer : public Layer{
		//VARIABLES
	private:
		ecs::Entity camera;
		std::vector<ecs::Entity> entities;

		//FUNCTIONS
	public:
		virtual void onAttach() override;
		virtual void onUpdate(utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;

		ecs::Entity addEntity();
	};
}