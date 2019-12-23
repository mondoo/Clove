#include "EditorLayer.hpp"

#include <Clove/Core/Platform/Application.hpp>
#include <Clove/Core/Platform/Window.hpp>
#include <Clove/Core/ECS/Manager.hpp>
#include <Clove/Core/ECS/3D/Components/TransformComponent.hpp>
#include <Clove/Core/ECS/3D/Components/CameraComponent.hpp>
#include <Clove/Core/ECS/3D/Components/MeshComponent.hpp>
#include <Clove/Core/Graphics/Renderables/Mesh.hpp>
#include <Clove/Core/Graphics/Material.hpp>

namespace clv::blb{
	void EditorLayer::onAttach(){
		const auto windowVP = clv::gfx::Viewport{ 0, 0, clv::plt::Application::get().getWindow().getSize().x, clv::plt::Application::get().getWindow().getSize().y };

		camera = plt::Application::get().getManager().createEntity();
		camera.addComponent<ecs::_3D::TransformComponent>()->setPosition({ 0.0f, 0.0f, -20.0f });
		auto* camComp = camera.addComponent<ecs::_3D::CameraComponent>(windowVP);

		clv::plt::Application::get().getWindow().onWindowResize.bind(&clv::ecs::_3D::CameraComponent::updateViewportSize, camComp);
	}

	void EditorLayer::onUpdate(utl::DeltaTime deltaTime){
		
	}

	void EditorLayer::onDetach(){
		auto& ecsManager = plt::Application::get().getManager();

		ecsManager.destroyEntity(camera.getID());

		for(auto& entity : entities){
			ecsManager.destroyEntity(entity.getID());
		}
	}

	void EditorLayer::addEntity(){
		auto cubeMaterial = std::make_shared<clv::gfx::Material>();
		cubeMaterial->setData(clv::gfx::BBP_Colour, clv::mth::vec4f{ 0.4f, 0.4f, 0.4f, 1.0f }, clv::gfx::ShaderType::Pixel);
		cubeMaterial->setData(clv::gfx::BBP_MaterialData, clv::gfx::MaterialData{ 32.0f }, clv::gfx::ShaderType::Pixel);
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", cubeMaterial->createInstance());

		ecs::Entity entity = clv::plt::Application::get().getManager().createEntity();
		entity.addComponent<clv::ecs::_3D::MeshComponent>(mesh);
		entity.addComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ 0.0f, 0.0f, 0.0f });;

		entities.push_back(entity);
	}
}