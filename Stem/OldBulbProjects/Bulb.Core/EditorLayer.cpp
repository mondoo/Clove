#include "EditorLayer.hpp"

#include <Tunic/Application.hpp>
#include <Clove/Platform/Core/Window.hpp>
#include <Tunic/ECS/Core/World.hpp>
#include <Tunic/ECS/3D/Components/TransformComponent.hpp>
#include <Tunic/ECS/3D/Components/CameraComponent.hpp>
#include <Tunic/ECS/3D/Components/MeshComponent.hpp>

namespace blb{
	void EditorLayer::onAttach(){
		const auto windowVP = clv::gfx::Viewport{ 0, 0, tnc::Application::get().getMainWindow().getSize().x, tnc::Application::get().getMainWindow().getSize().y };

		camera = tnc::Application::get().getWorld().createEntity();
		camera.addComponent<tnc::ecs::_3D::TransformComponent>()->setPosition({ 0.0f, 0.0f, -20.0f });
		auto* camComp = camera.addComponent<tnc::ecs::_3D::CameraComponent>(windowVP, tnc::ecs::_3D::ProjectionMode::perspective);

		tnc::Application::get().getMainWindow().onWindowResize.bind(&tnc::ecs::_3D::CameraComponent::updateViewportSize, camComp);
	}

	void EditorLayer::onUpdate(clv::utl::DeltaTime deltaTime){
		
	}

	void EditorLayer::onDetach(){
		auto& ecsWorld = tnc::Application::get().getWorld();

		ecsWorld.destroyEntity(camera.getID());

		for(auto& entity : entities){
			ecsWorld.destroyEntity(entity.getID());
		}
	}

	tnc::ecs::Entity EditorLayer::addEntity(){
		tnc::ecs::Entity entity = tnc::Application::get().getWorld().createEntity();
		entities.push_back(entity);
		return entity;
	}
}