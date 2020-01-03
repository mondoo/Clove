#pragma once

#include <Clove/Core/Layer.hpp>
#include <Tunic/ECS/Core/Entity.hpp>

namespace clv::blb{
	class EditorLayer : public Layer{
		//VARIABLES
	private:
		tnc::ecs::Entity camera;
		std::vector<tnc::ecs::Entity> entities;

		//FUNCTIONS
	public:
		virtual void onAttach() override;
		virtual void onUpdate(utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;

		tnc::ecs::Entity addEntity();
	};
}