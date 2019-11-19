#pragma once

#include "Core/Layer.hpp"

#include "Core/ECS/Entity.hpp"

class TestLayer : public clv::Layer{
	//VARIABLES
private:
	float rotDelta = 0.0f;

	clv::ecs::Entity ent1;
	clv::ecs::Entity ent2;
	clv::ecs::Entity ent3;

	clv::ecs::Entity bigBoy;

	clv::ecs::Entity rtEnt;

	clv::ecs::Entity sprtEnt1;
	clv::ecs::Entity sprtEnt2;

	clv::ecs::Entity lght1;
	clv::ecs::Entity lght2;

	clv::ecs::Entity cam;
	clv::ecs::Entity subcam;

	clv::ecs::Entity sound;

	clv::ecs::Entity fontEnt;
	clv::ecs::Entity fpsEnt;

	clv::ecs::Entity rigidBody1;
	clv::ecs::Entity rigidBody2;

	clv::ecs::Entity rigidSprite1;
	clv::ecs::Entity rigidSprite2;

	bool firstMouse = false;
	float pitch = 0.0f;
	float yaw = 0.0f;
	int32_t lastMouseX = 0;
	int lastMouseY = 0;

	clv::uint64 countedFrames = 0;
	double secondsPassed = 0.0;

	//FUNCTIONS
public:
	TestLayer();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;
};