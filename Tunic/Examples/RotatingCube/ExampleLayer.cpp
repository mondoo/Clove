#include "ExampleLayer.hpp"

#include <Tunic/Tunic.hpp>

ExampleLayer::ExampleLayer() {
	//Add on any systems we want to use
	world.addSystem<tnc::ecs::_3D::RenderSystem>(tnc::Application::get().getMainWindow());
}

void ExampleLayer::onAttach() {
	clv::plt::Window& window = tnc::Application::get().getMainWindow();

	//Load in our mesh
	tnc::rnd::Model cube = tnc::ModelLoader::loadModel(SOURCE_DIR "/cube.obj");

	//Create the entity that will act as our cube
	cubeEntity = world.createEntity();
	//Add components that will define our entity and interact with certain systems
	cubeEntity.addComponent<tnc::ecs::_3D::TransformComponent>();
	cubeEntity.addComponent<tnc::ecs::_3D::ModelComponent>(cube);

	//Add our light entity
	lightEntity = world.createEntity();
	//The direction is where the light is pointing. In our case down to the left and back
	lightEntity.addComponent<tnc::ecs::_3D::DirectionalLightComponent>()->setDirection({ 1.0f, -1.0f, 1.0f });

	//Add our camera entity
	cameraEntity = world.createEntity();
	cameraEntity.addComponent<tnc::ecs::_3D::TransformComponent>()->setPosition({ 0.0f, 0.0f, -5.0f });
	//The CameraComponent requires a window or a render target to render to
	cameraEntity.addComponent<tnc::ecs::_3D::CameraComponent>(window, tnc::ecs::_3D::ProjectionMode::perspective);
}

void ExampleLayer::onUpdate(clv::utl::DeltaTime deltaTime) {
	constexpr float rotationSpeed = 0.5f;
	static float rotation = 0.0f;

	//Retrieve the transform component from the cube entity
	auto* cubeTransform = cubeEntity.getComponent<tnc::ecs::_3D::TransformComponent>();

	//Rotate the cube around the Y axis 
	rotation += rotationSpeed * deltaTime;
	cubeTransform->setRotation(clv::mth::eulerToQuaternion(clv::mth::vec3f{ 0.0f, rotation, 0.0f }));

	//Update the ECS world
	world.update(deltaTime);
}

void ExampleLayer::onDetach() {
	//Make sure to clear any entities when this layer detaches as that might not be when the layer is destroyed
	world.destroyAllEntites();
}