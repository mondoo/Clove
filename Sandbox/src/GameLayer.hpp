#pragma once

#include "Clove/Layer.hpp"

#include "Clove/ECS/Entity.hpp"

class GameLayer : public clv::Layer{
	//VARAIBLES
private:
	clv::ecs::Entity cameraEntity;
	clv::ecs::Entity sphereEntity;

	clv::ecs::Entity tempLight;

	//FUNCTIONS
public:
	GameLayer();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;

	virtual void onEvent(clv::Event& e) override;
};