#include "EditorLayer.hpp"

#include <Tunic/Application.hpp>
#include <Clove/Core/Platform/Window.hpp>
#include <Tunic/ECS/Core/Manager.hpp>
#include <Tunic/ECS/3D/Components/TransformComponent.hpp>
#include <Tunic/ECS/3D/Components/CameraComponent.hpp>
#include <Tunic/ECS/3D/Components/MeshComponent.hpp>
#include <Clove/Core/Graphics/Renderables/Mesh.hpp>
#include <Clove/Core/Graphics/Material.hpp>

namespace blb{
	void EditorLayer::onAttach(){
		const auto windowVP = clv::gfx::Viewport{ 0, 0, tnc::Application::get().getMainWindow().getSize().x, tnc::Application::get().getMainWindow().getSize().y };

		camera = tnc::Application::get().getManager().createEntity();
		camera.addComponent<tnc::ecs::_3D::TransformComponent>()->setPosition({ 0.0f, 0.0f, -20.0f });
		auto* camComp = camera.addComponent<tnc::ecs::_3D::CameraComponent>(windowVP);

		tnc::Application::get().getMainWindow().onWindowResize.bind(&tnc::ecs::_3D::CameraComponent::updateViewportSize, camComp);
	}

	void EditorLayer::onUpdate(clv::utl::DeltaTime deltaTime){
		
	}

	void EditorLayer::onDetach(){
		auto& ecsManager = tnc::Application::get().getManager();

		ecsManager.destroyEntity(camera.getID());

		for(auto& entity : entities){
			ecsManager.destroyEntity(entity.getID());
		}
	}

	tnc::ecs::Entity EditorLayer::addEntity(){
		tnc::ecs::Entity entity = tnc::Application::get().getManager().createEntity();
		entities.push_back(entity);
		return entity;
	}
}