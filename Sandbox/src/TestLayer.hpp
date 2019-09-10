#pragma once

#include "Clove/Layer.hpp"

#include "Clove/ECS/Entity.hpp"

class TestLayer : public clv::Layer{
	//VARIABLES
private:
	float rotDelta = 0.0f;

	clv::ecs::Entity ent1;
	clv::ecs::Entity ent2;
	clv::ecs::Entity ent3;

	clv::ecs::Entity rtEnt;

	clv::ecs::Entity sprtEnt1;
	clv::ecs::Entity sprtEnt2;

	clv::ecs::Entity lght1;
	clv::ecs::Entity lght2;

	clv::ecs::Entity cam;

	clv::ecs::Entity sound;

	bool firstMouse = false;
	float pitch = 0.0f;
	float yaw = 0.0f;
	int32_t lastMouseX = 0;
	int lastMouseY = 0;

	//FUNCTIONS
public:
	TestLayer();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;
};