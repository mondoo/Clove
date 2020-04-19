#pragma once

#include <Tunic/Layer.hpp>

#include <Tunic/ECS/World.hpp>
#include <Tunic/ECS/Entity.hpp>

namespace clv::plt {
	class Window;
}

class ExampleLayer : public tnc::Layer {
	//VARIABLES
private:
	//The window we want to render to
	std::shared_ptr<clv::plt::Window> window;

	//The ECS world that this layer has
	tnc::ecs::World world;

	//The entities we want in our world
	tnc::ecs::Entity cubeEntity;
	tnc::ecs::Entity lightEntity;
	tnc::ecs::Entity cameraEntity;

	//FUNCTIONS
public:
	ExampleLayer(std::shared_ptr<clv::plt::Window> window);

	//Called when the layer is attached
	virtual void onAttach() override;

	//Called once per frame
	virtual void onUpdate(clv::utl::DeltaTime deltaTime) override;

	//Called when the layer has been detached
	virtual void onDetach() override;
};