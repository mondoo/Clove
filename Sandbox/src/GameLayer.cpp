#include "GameLayer.hpp"

#include "Core/ECS/3D/Components/CameraComponent.hpp"
#include "Core/ECS/3D/Components/MeshComponent.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/Graphics/Renderables/Mesh.hpp"
#include "Core/Graphics/Material.hpp"
#include "Core/ECS/3D/Components/LightComponent.hpp"

#include "Core/Input/Input.hpp"

GameLayer::GameLayer()
	: Layer("Game Layer"){
}

void GameLayer::onAttach(){
	//cameraEntity = clv::Application::get().getManager().createEntity<clv::ecs::_3D::CameraComponent, clv::ecs::_3D::TransformComponent>();
	//sphereEntity = clv::Application::get().getManager().createEntity<clv::ecs::_3D::MeshComponent, clv::ecs::_3D::TransformComponent>();
	//
	//tempCube = clv::Application::get().getManager().createEntity<clv::ecs::_3D::MeshComponent, clv::ecs::_3D::TransformComponent>();
	//tempLight = clv::Application::get().getManager().createEntity<clv::ecs::_3D::LightComponent, clv::ecs::_3D::TransformComponent>();

	//sphereEntity.getComponent<clv::ecs::_3D::TransformComponent>()->addChild(cameraEntity.getComponent<clv::ecs::_3D::TransformComponent>());
	//cameraEntity.getComponent<clv::ecs::_3D::TransformComponent>()->setLocalPosition({ -5.0f, 0.0f, 0.0f });

	//auto sphereMesh = std::make_shared<clv::gfx::Mesh>("res/Objects/sphere.obj");
	//auto sphereMaterial = std::make_shared<clv::gfx::Material>();
	//sphereMesh->setMaterial(sphereMaterial);
	//sphereMaterial->setAlbedoTexture("res/Textures/container2.png");

	//sphereEntity.getComponent<clv::ecs::_3D::MeshComponent>()->setMesh(sphereMesh);
	////sphereEntity.getComponent<clv::ecs::_3D::MeshComponent>()->setAlbedoTexture("res/Textures/container2.png");
	////sphereEntity.getComponent<clv::ecs::MeshComponent>()->setSpecularTexture("res/Textures/container2_specular.png");

	//auto cubeMesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj");
	//auto cubeMaterial = std::make_shared<clv::gfx::Material>();
	//cubeMesh->setMaterial(sphereMaterial);
	//cubeMaterial->setAlbedoTexture("res/Textures/container2.png");

	//tempCube.getComponent<clv::ecs::_3D::RenderableComponent>()->setMesh(cubeMesh);
	////tempCube.getComponent<clv::ecs::_3D::RenderableComponent>()->setAlbedoTexture("res/Textures/container2.png");
}

void GameLayer::onDetach(){

}

void GameLayer::onUpdate(clv::utl::DeltaTime deltaTime){
	/*if(clv::input::isKeyPressed(clv::Key::W)){
		clv::mth::vec3f spherePos = sphereEntity.getComponent<clv::ecs::_3D::TransformComponent>()->getPosition();
		spherePos.x += 5 * deltaTime;
		sphereEntity.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition(spherePos);
	}else if(clv::input::isKeyPressed(clv::Key::S)){
		clv::mth::vec3f spherePos = sphereEntity.getComponent<clv::ecs::_3D::TransformComponent>()->getPosition();
		spherePos.x -= 5 * deltaTime;
		sphereEntity.getComponent<clv::ecs::_3D::TransformComponent>()->setPosition(spherePos);
	}*/
}