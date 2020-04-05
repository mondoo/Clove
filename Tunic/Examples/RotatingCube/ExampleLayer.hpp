#pragma once

#include <Tunic/Layer.hpp>

#include<Tunic/ECS/Core/Entity.hpp>

class ExampleLayer : public tnc::Layer {
	//VARIABLES
private:
	tnc::ecs::Entity cubeEntity;
	tnc::ecs::Entity lightEntity;
	tnc::ecs::Entity cameraEntity;

	//FUNCTIONS
public:
	//Called when the layer is attached
	virtual void onAttach() override;

	//Called once per frame
	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;

	//Called when the layer has been detached
	virtual void onDetach() override;
};