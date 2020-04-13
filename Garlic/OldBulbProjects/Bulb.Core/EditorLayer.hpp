#pragma once

#include <Clove/Core/Layer.hpp>
#include <Tunic/ECS/Core/Entity.hpp>

namespace blb{
	class EditorLayer : public clv::Layer{
		//VARIABLES
	private:
		tnc::ecs::Entity camera;
		std::vector<tnc::ecs::Entity> entities;

		//FUNCTIONS
	public:
		virtual void onAttach() override;
		virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;
		virtual void onDetach() override;

		tnc::ecs::Entity addEntity();
	};
}