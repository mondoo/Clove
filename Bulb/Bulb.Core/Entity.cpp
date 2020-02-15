#include "Entity.hpp"

#include <Tunic/Application.hpp>
#include <Tunic/ECS/Core/World.hpp>
#include <Tunic/ECS/3D/Components/TransformComponent.hpp>
#include <Tunic/ECS/3D/Components/MeshComponent.hpp>
#include <Tunic/Rendering/Renderables/Mesh.hpp>
#include <Tunic/Rendering/Material.hpp>

namespace Bulb::Core{
	Entity::Entity(tnc::ecs::EntityID ID){
		this->ID = ID;
		components = gcnew System::Collections::Generic::List<Component^>();
	}

	System::Collections::Generic::List<Component^>^ Entity::getComponents(){
		return components;
	}

	TransformComponent^ Entity::addTransformComponent(){
		auto* trans = tnc::Application::get().getWorld().addComponent<tnc::ecs::_3D::TransformComponent>(ID);

		auto comp = gcnew TransformComponent(trans);
		comp->name = "Transform Component";
		components->Add(comp);
		return comp;
	}

	Component^ Entity::addMeshComponent(){
		auto material = std::make_shared<tnc::rnd::Material>();
		auto mesh = std::make_shared<tnc::rnd::Mesh>("res/Objects/cube.obj", material->createInstance());
		tnc::Application::get().getWorld().addComponent<tnc::ecs::_3D::MeshComponent>(ID, mesh);

		auto comp = gcnew Component();
		comp->name = "Mesh Component";
		components->Add(comp);
		return comp;
	}
}