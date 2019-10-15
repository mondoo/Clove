#include "GameLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/ECS/3D/Components/CameraComponent.hpp"
#include "Clove/ECS/3D/Components/RenderableComponent.hpp"
#include "Clove/ECS/3D/Components/TransformComponent.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/ECS/3D/Components/LightComponent.hpp"

#include "Clove/Input/Input.hpp"

GameLayer::GameLayer()
	: Layer("Game Layer"){
}

void GameLayer::onAttach(){
	//cameraEntity = clv::Application::get().getManager().createEntity<clv::ecs::d3::CameraComponent, clv::ecs::d3::TransformComponent>();
	//sphereEntity = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();
	//
	//tempCube = clv::Application::get().getManager().createEntity<clv::ecs::d3::RenderableComponent, clv::ecs::d3::TransformComponent>();
	//tempLight = clv::Application::get().getManager().createEntity<clv::ecs::d3::LightComponent, clv::ecs::d3::TransformComponent>();

	//sphereEntity.getComponent<clv::ecs::d3::TransformComponent>()->addChild(cameraEntity.getComponent<clv::ecs::d3::TransformComponent>());
	//cameraEntity.getComponent<clv::ecs::d3::TransformComponent>()->setLocalPosition({ -5.0f, 0.0f, 0.0f });

	//auto sphereMesh = std::make_shared<clv::gfx::Mesh>("res/Objects/sphere.obj");
	//auto sphereMaterial = std::make_shared<clv::gfx::Material>();
	//sphereMesh->setMaterial(sphereMaterial);
	//sphereMaterial->setAlbedoTexture("res/Textures/container2.png");

	//sphereEntity.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(sphereMesh);
	////sphereEntity.getComponent<clv::ecs::d3::RenderableComponent>()->setAlbedoTexture("res/Textures/container2.png");
	////sphereEntity.getComponent<clv::ecs::MeshComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	//auto cubeMesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj");
	//auto cubeMaterial = std::make_shared<clv::gfx::Material>();
	//cubeMesh->setMaterial(sphereMaterial);
	//cubeMaterial->setAlbedoTexture("res/Textures/container2.png");

	//tempCube.getComponent<clv::ecs::d3::RenderableComponent>()->setMesh(cubeMesh);
	////tempCube.getComponent<clv::ecs::d3::RenderableComponent>()->setAlbedoTexture("res/Textures/container2.png");
}

void GameLayer::onDetach(){

}

void GameLayer::onUpdate(clv::utl::DeltaTime deltaTime){
	/*if(clv::input::isKeyPressed(clv::Key::W)){
		clv::math::Vector3f spherePos = sphereEntity.getComponent<clv::ecs::d3::TransformComponent>()->getPosition();
		spherePos.x += 5 * deltaTime;
		sphereEntity.getComponent<clv::ecs::d3::TransformComponent>()->setPosition(spherePos);
	}else if(clv::input::isKeyPressed(clv::Key::S)){
		clv::math::Vector3f spherePos = sphereEntity.getComponent<clv::ecs::d3::TransformComponent>()->getPosition();
		spherePos.x -= 5 * deltaTime;
		sphereEntity.getComponent<clv::ecs::d3::TransformComponent>()->setPosition(spherePos);
	}*/
}