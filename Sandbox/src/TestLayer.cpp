#include "clvpch.hpp" //will generate a tonne of compile errors with the pch - should I remove it from the library?
#include "TestLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/ECS/Manager.hpp"
#include "Clove/ECS/3D/Components/RenderableComponent.hpp"
#include "Clove/ECS/2D/Components/RenderableComponent.hpp"
#include "Clove/ECS/2D/Components/TransformComponent.hpp"
#include "Clove/ECS/3D/Components/TransformComponent.hpp"
#include "Clove/ECS/3D/Components/CameraComponent.hpp"
#include "Clove/ECS/3D/Components/LightComponent.hpp"
#include "Clove/Maths/Vector.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/Utils/DeltaTime.hpp"

TestLayer::TestLayer()
	: Layer("Example Layer"){
}

void TestLayer::onAttach(){
	ent1 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();
	ent2 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();
	ent3 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();

	sprtEnt1 = clv::Application::get().getManager().createEntity<clv::ecs::d2::RenderableComponent, clv::ecs::d2::TransformComponent>();
	sprtEnt2 = clv::Application::get().getManager().createEntity<clv::ecs::d2::RenderableComponent, clv::ecs::d2::TransformComponent>();

	lght1 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::LightComponent, clv::ecs::d3::TransformComponent>();
	lght2 = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::LightComponent, clv::ecs::d3::TransformComponent>();

	cam = clv::Application::get().getManager().createEntity<clv::ecs::d3::CameraComponent, clv::ecs::d3::TransformComponent>();
	
	ent1.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh("res/Objects/cube.obj");
	ent1.getComponent<clv::ecs::d3::RenderableComponent>()->setDiffuseTexture("res/Textures/container2.png");
	ent1.getComponent<clv::ecs::d3::RenderableComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	ent2.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh("res/Objects/cube.obj");
	ent2.getComponent<clv::ecs::d3::RenderableComponent>()->setDiffuseTexture("res/Textures/container2.png");
	ent2.getComponent<clv::ecs::d3::RenderableComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	ent3.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh("res/Objects/cube.obj");
	ent3.getComponent<clv::ecs::d3::RenderableComponent>()->setDiffuseTexture("res/Textures/container2.png");
	ent3.getComponent<clv::ecs::d3::RenderableComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ 0.0f, 0.0f, 0.0f });
	ent2.getComponent<clv::ecs::d3::TransformComponent>()->setLocalPosition({ 0.0f, 0.0f, 3.0f });
	ent3.getComponent<clv::ecs::d3::TransformComponent>()->setLocalPosition({ 0.0f, 3.0f, 0.0f });

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->addChild(ent2.getComponent<clv::ecs::d3::TransformComponent>());
	ent2.getComponent<clv::ecs::d3::TransformComponent>()->addChild(ent3.getComponent<clv::ecs::d3::TransformComponent>());

	//clv::Application::get().getManager().destroyEntity(ent2.getID());

	sprtEnt1.getComponent<clv::ecs::d2::RenderableComponent>()->setTexture("res/Textures/Zombie-32x32.png");
	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setScale(clv::math::Vector2f(32.0f, 32.0f));

	sprtEnt2.getComponent<clv::ecs::d2::RenderableComponent>()->setTexture("res/Textures/Zombie-32x32.png");
	sprtEnt2.getComponent<clv::ecs::d2::TransformComponent>()->setLocalPosition(clv::math::Vector2f(0.0f, 1.0f));

	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->addChild(sprtEnt2.getComponent<clv::ecs::d2::TransformComponent>());

	lght1.getComponent<clv::ecs::d3::TransformComponent>()->setScale({ 0.25f, 0.25f, 0.25f });
	lght1.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh("res/Objects/cube.obj");
	lght1.getComponent<clv::ecs::d3::RenderableComponent>()->setDiffuseTexture("res/Textures/container2.png");
	lght1.getComponent<clv::ecs::d3::RenderableComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	lght2.getComponent<clv::ecs::d3::TransformComponent>()->setScale({ 0.25f, 0.25f, 0.25f });
	lght2.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh("res/Objects/cube.obj");
	lght2.getComponent<clv::ecs::d3::RenderableComponent>()->setDiffuseTexture("res/Textures/container2.png");
	lght2.getComponent<clv::ecs::d3::RenderableComponent>()->setSpecularTexture("res/Textures/container2_specular.png");
}

void TestLayer::onDetach(){

}

void TestLayer::onUpdate(clv::utl::DeltaTime deltaTime){
	const float camSpeed = 15.0f;
	const float camDelta = camSpeed * deltaTime;
	const float rotSpeed = 1.5f;
	//const float deltaTime = deltaTime;

	float s = 6 * deltaTime;

	clv::math::Vector3f cameraPosition = cam.getComponent<clv::ecs::d3::TransformComponent>()->getPosition();

	//cam->updateFront(pitch, yaw); //TODO: proper first person implementation

	const clv::math::Vector3f front = cam.getComponent<clv::ecs::d3::CameraComponent>()->getFront();
	if(clv::input::isKeyPressed(clv::Key::W)){
		cameraPosition += front * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::S)){
		cameraPosition -= front * camDelta;
	}

	const clv::math::Vector3f up = cam.getComponent<clv::ecs::d3::CameraComponent>()->getUp();
	if(clv::input::isKeyPressed(clv::Key::Space)){
		cameraPosition += up * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::C)){
		cameraPosition -= up * camDelta;
	}

	const clv::math::Vector3f right = cam.getComponent<clv::ecs::d3::CameraComponent>()->getRight();
	if(clv::input::isKeyPressed(clv::Key::A)){
		cameraPosition -= right * camDelta;
	} else if(clv::input::isKeyPressed(clv::Key::D)){
		cameraPosition += right * camDelta;
	}

	if(clv::input::isKeyPressed(clv::Key::Q)){
		yaw -= camDelta * 10.0f;
	} else if(clv::input::isKeyPressed(clv::Key::E)){
		yaw += camDelta * 10.0f;
	}

	const float radius = 6.0f;
	
	cam.getComponent<clv::ecs::d3::TransformComponent>()->setPosition(cameraPosition);
	cam.getComponent<clv::ecs::d3::CameraComponent>()->updateFront(0.0f, yaw);

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->setRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 0.0f, 1.0f, 0.0f }));
	ent2.getComponent<clv::ecs::d3::TransformComponent>()->setLocalRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 0.0f, 0.0f, 1.0f }));
	ent3.getComponent<clv::ecs::d3::TransformComponent>()->setLocalRotation(clv::math::asQuaternion(rotDelta, clv::math::Vector3f{ 1.0f, 0.0f, 0.0f }));

	ent1.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ cos(rotDelta) * radius, 0.0f, 0.0f });

	lght1.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ cos(rotDelta * 1.5f) * radius * 2.0f, 0.0f, sin(rotDelta * 1.5f) * radius * 2.0f });
	lght2.getComponent<clv::ecs::d3::TransformComponent>()->setPosition({ cos(rotDelta) * radius * 2.0f, sin(rotDelta) * radius * 2.0f, 0.0f });

	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setPosition(clv::math::Vector2f(cos(rotDelta) * radius * 5.0f, 0.0f));
	sprtEnt1.getComponent<clv::ecs::d2::TransformComponent>()->setRotation(rotDelta);

	rotDelta += rotSpeed * deltaTime;

	if(clv::input::isKeyPressed(clv::Key::Escape)){
		clv::Application::get().stop();
	}
}

//bool TestLayer::onMouseMoved(clv::MouseMovedEvent e){
//	if(firstMouse){
//		lastMouseX = e.getX();
//		lastMouseY = e.getY();
//		firstMouse = false;
//	}
//
//	float xoffset = static_cast<float>(e.getX() - lastMouseX);
//	float yoffset = static_cast<float>(lastMouseY - e.getY());
//	lastMouseX = e.getX();
//	lastMouseY = e.getY();
//
//	float sensitivity = 0.05f;
//	xoffset *= sensitivity;
//	yoffset *= sensitivity;
//
//	yaw += xoffset;
//	pitch += yoffset;
//
//	if(pitch > 89.0f){
//		pitch = 89.0f;
//	}
//	if(pitch < -89.0f){
//		pitch = -89.0f;
//	}
//
//	return true;
//}