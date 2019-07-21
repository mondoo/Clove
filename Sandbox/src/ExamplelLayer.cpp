#include "clvpch.hpp" //will generate a tonne of compile errors with the pch - should I remove it from the library?
#include "ExampleLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/ECS/Manager.hpp"
#include "Clove/ECS/Components/MeshComponent.hpp"
#include "Clove/ECS/Components/SpriteComponent.hpp"
#include "Clove/ECS/Components/Transform2DComponent.hpp"
#include "Clove/ECS/Components/Transform3DComponent.hpp"
#include "Clove/ECS/Components/CameraComponent.hpp"
#include "Clove/ECS/Components/LightComponent.hpp"
#include "Clove/Maths/Vector.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/Utils/Time.hpp"

ExampleLayer::ExampleLayer()
	: Layer("Example Layer"){
}

void ExampleLayer::onAttach(){
	ent1 = clv::Application::get().getManager().createEntity<clv::ecs::MeshComponent, clv::ecs::Transform3DComponent>();
	ent2 = clv::Application::get().getManager().createEntity<clv::ecs::MeshComponent, clv::ecs::Transform3DComponent>();
	ent3 = clv::Application::get().getManager().createEntity<clv::ecs::MeshComponent, clv::ecs::Transform3DComponent>();

	sprtEnt1 = clv::Application::get().getManager().createEntity<clv::ecs::SpriteComponent, clv::ecs::Transform2DComponent>();

	lght1 = clv::Application::get().getManager().createEntity<clv::ecs::MeshComponent, clv::ecs::LightComponent, clv::ecs::Transform3DComponent>();

	cam = clv::Application::get().getManager().createEntity<clv::ecs::CameraComponent, clv::ecs::Transform3DComponent>();
	
	ent1.getComponent<clv::ecs::MeshComponent>()->setMesh("res/Objects/cube.obj");
	ent1.getComponent<clv::ecs::MeshComponent>()->setDiffuseTexture("res/Textures/container2.png");
	ent1.getComponent<clv::ecs::MeshComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	ent2.getComponent<clv::ecs::MeshComponent>()->setMesh("res/Objects/cube.obj");
	ent2.getComponent<clv::ecs::MeshComponent>()->setDiffuseTexture("res/Textures/container2.png");
	ent2.getComponent<clv::ecs::MeshComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	ent3.getComponent<clv::ecs::MeshComponent>()->setMesh("res/Objects/cube.obj");
	ent3.getComponent<clv::ecs::MeshComponent>()->setDiffuseTexture("res/Textures/container2.png");
	ent3.getComponent<clv::ecs::MeshComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	ent1.getComponent<clv::ecs::Transform3DComponent>()->setLocalPosition({ 0.0f, 0.0f, 0.0f });
	ent2.getComponent<clv::ecs::Transform3DComponent>()->setLocalPosition({ 0.0f, 0.0f, 3.0f });
	ent3.getComponent<clv::ecs::Transform3DComponent>()->setLocalPosition({ 0.0f, 3.0f, 0.0f });

	ent1.getComponent<clv::ecs::Transform3DComponent>()->addChild(ent2.getComponent<clv::ecs::Transform3DComponent>());
	ent2.getComponent<clv::ecs::Transform3DComponent>()->addChild(ent3.getComponent<clv::ecs::Transform3DComponent>());

	//clv::Application::get().getManager().destroyEntity(ent2.getID());

	sprtEnt1.getComponent<clv::ecs::SpriteComponent>()->setTexture("res/Textures/Zombie-32x32.png");
	sprtEnt1.getComponent<clv::ecs::Transform2DComponent>()->setLocalScale(clv::math::Vector2f(32.0f, 32.0f));

	lght1.getComponent<clv::ecs::Transform3DComponent>()->setLocalScale({ 0.25f, 0.25f, 0.25f });
	lght1.getComponent<clv::ecs::MeshComponent>()->setMesh("res/Objects/cube.obj");
	lght1.getComponent<clv::ecs::MeshComponent>()->setDiffuseTexture("res/Textures/container2.png");
	lght1.getComponent<clv::ecs::MeshComponent>()->setSpecularTexture("res/Textures/container2_specular.png");
}

void ExampleLayer::onDetach(){

}

void ExampleLayer::onUpdate(){
	const float camSpeed = 15.0f;
	const float rotSpeed = 1.5f;
	const float deltaTime = clv::utl::Time::getDeltaTime();

	clv::math::Vector3f cameraPosition = cam.getComponent<clv::ecs::Transform3DComponent>()->getLocalPosition();

	//cam->updateFront(pitch, yaw); //TODO: proper first person implementation

	const clv::math::Vector3f front = cam.getComponent<clv::ecs::CameraComponent>()->getFront();
	if(clv::input::isKeyPressed(clv::Key::W)){
		cameraPosition += camSpeed * front * deltaTime;
	} else if(clv::input::isKeyPressed(clv::Key::S)){
		cameraPosition -= camSpeed * front * deltaTime;
	}

	const clv::math::Vector3f up = cam.getComponent<clv::ecs::CameraComponent>()->getUp();
	if(clv::input::isKeyPressed(clv::Key::Space)){
		cameraPosition += camSpeed * up * deltaTime;
	} else if(clv::input::isKeyPressed(clv::Key::C)){
		cameraPosition -= camSpeed * up * deltaTime;
	}

	const clv::math::Vector3f right = cam.getComponent<clv::ecs::CameraComponent>()->getRight();
	if(clv::input::isKeyPressed(clv::Key::A)){
		cameraPosition -= camSpeed * right * deltaTime;
	} else if(clv::input::isKeyPressed(clv::Key::D)){
		cameraPosition += camSpeed * right * deltaTime;
	}

	if(clv::input::isKeyPressed(clv::Key::Q)){
		yaw -= camSpeed * 10.0f * deltaTime;
	} else if(clv::input::isKeyPressed(clv::Key::E)){
		yaw += camSpeed * 10.0f * deltaTime;
	}

	cam.getComponent<clv::ecs::Transform3DComponent>()->setLocalPosition(cameraPosition);
	cam.getComponent<clv::ecs::CameraComponent>()->updateFront(0.0f, yaw);

	ent1.getComponent<clv::ecs::Transform3DComponent>()->setLocalRotation({ { 0.0f, 1.0f, 0.0f }, rotDelta });
	ent2.getComponent<clv::ecs::Transform3DComponent>()->setLocalRotation({ { 0.0f, 0.0f, 1.0f }, rotDelta });
	ent3.getComponent<clv::ecs::Transform3DComponent>()->setLocalRotation({ { 1.0f, 0.0f, 0.0f }, rotDelta });

	const float radius = 6.0f;
	lght1.getComponent<clv::ecs::Transform3DComponent>()->setLocalPosition({ cos(rotDelta) * radius, 2.0f, sin(rotDelta) * radius });

	sprtEnt1.getComponent<clv::ecs::Transform2DComponent>()->setLocalPosition(clv::math::Vector2f(cos(rotDelta) * radius * 5.0f, 0.0f));

	rotDelta += rotSpeed * deltaTime;

	if(clv::input::isKeyPressed(clv::Key::Escape)){
		clv::Application::get().stop();
	}
}

void ExampleLayer::onEvent(clv::Event& e){
	//clv::EventDispatcher dispatcher(e);
	//dispatcher.dispatch<clv::MouseMovedEvent>(CLV_BIND_FUNCTION_1P(&ExampleLayer::onMouseMoved, this));
}

bool ExampleLayer::onMouseMoved(clv::MouseMovedEvent e){
	if(firstMouse){
		lastMouseX = e.getX();
		lastMouseY = e.getY();
		firstMouse = false;
	}

	float xoffset = static_cast<float>(e.getX() - lastMouseX);
	float yoffset = static_cast<float>(lastMouseY - e.getY());
	lastMouseX = e.getX();
	lastMouseY = e.getY();

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f){
		pitch = 89.0f;
	}
	if(pitch < -89.0f){
		pitch = -89.0f;
	}

	return true;
}