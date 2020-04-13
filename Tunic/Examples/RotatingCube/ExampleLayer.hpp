#pragma once

#include <Tunic/Layer.hpp>

#include <Tunic/ECS/World.hpp>
#include <Tunic/ECS/Entity.hpp>

class ExampleLayer : public tnc::Layer {
	//VARIABLES
private:
	//The ECS world that this layer has
	tnc::ecs::World world;

	//The entities we want in our world
	tnc::ecs::Entity cubeEntity;
	tnc::ecs::Entity lightEntity;
	tnc::ecs::Entity cameraEntity;

	//FUNCTIONS
public:
	ExampleLayer();

	//Called when the layer is attached
	virtual void onAttach() override;

	//Called once per frame
	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;

	//Called when the layer has been detached
	virtual void onDetach() override;
};