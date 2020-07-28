#include "ExampleLayer.hpp"

#include <Bulb/Bulb.hpp>

ExampleLayer::ExampleLayer(std::shared_ptr<clv::plt::Window> window) 
	: window(std::move(window)) {
	//Initialise our renderer
    renderer = std::make_shared<blb::rnd::Renderer3D>(*this->window);

	//Add on any systems we want to use
    world.addSystem<blb::ecs::RenderSystem>(renderer);
}

void ExampleLayer::onAttach() {
	//Load in our mesh
	blb::rnd::StaticModel cube = blb::ModelLoader::loadStaticModel(SOURCE_DIR "/cube.obj", window->getGraphicsFactory());

	//Create the entity that will act as our cube
	cubeEntity = world.createEntity();
	//Add components that will define our entity and interact with certain systems
	cubeEntity.addComponent<blb::ecs::TransformComponent>();
	cubeEntity.addComponent<blb::ecs::StaticModelComponent>(cube);

	//Add our light entity
	lightEntity = world.createEntity();
	//The direction is where the light is pointing. In our case down to the left and back
	lightEntity.addComponent<blb::ecs::DirectionalLightComponent>()->setDirection({ 1.0f, -1.0f, 1.0f });

	//Add our camera entity
	cameraEntity = world.createEntity();
	cameraEntity.addComponent<blb::ecs::TransformComponent>()->setPosition({ 0.0f, 0.0f, -5.0f });
	//The CameraComponent requires a window or a render target to render to
    cameraEntity.addComponent<blb::ecs::CameraComponent>(blb::rnd::Camera{ *window, blb::rnd::ProjectionMode::Perspective });
}

void ExampleLayer::onUpdate(clv::utl::DeltaTime deltaTime) {
	constexpr float rotationSpeed = 0.5f;
	static float rotation = 0.0f;

	//Tell the renderer we're begining a frame
    renderer->begin();

	//Retrieve the transform component from the cube entity
	blb::ecs::ComponentPtr<blb::ecs::TransformComponent> cubeTransform = cubeEntity.getComponent<blb::ecs::TransformComponent>();

	//Rotate the cube around the Y axis 
	rotation += rotationSpeed * deltaTime;
	cubeTransform->setRotation(clv::mth::eulerToQuaternion(clv::mth::vec3f{ 0.0f, rotation, 0.0f }));

	//Update the ECS world
	world.update(deltaTime);

	//End our frame
    renderer->end();
}

void ExampleLayer::onDetach() {
	//Make sure to clear any entities when this layer detaches as that might not be when the layer is destroyed
	world.destroyAllEntites();
}