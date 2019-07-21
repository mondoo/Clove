#pragma once

#include "Clove/Layer.hpp"

#include "Clove/ECS/Entity.hpp"
#include "Clove/Events/MouseEvent.hpp"

class TestLayer : public clv::Layer{
	//VARIABLES
private:
	float rotDelta = 0.0f;

	clv::ecs::Entity ent1;
	clv::ecs::Entity ent2;
	clv::ecs::Entity ent3;

	clv::ecs::Entity sprtEnt1;

	clv::ecs::Entity lght1;

	clv::ecs::Entity cam;

	bool firstMouse = false;
	float pitch = 0.0f;
	float yaw = 0.0f;
	int lastMouseX = 0.0f;
	int lastMouseY = 0.0f;

	//FUNCTIONS
public:
	TestLayer();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate() override;

	virtual void onEvent(clv::Event& e) override;

	bool onMouseMoved(clv::MouseMovedEvent e);
};