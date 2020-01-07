#include "Entity.hpp"

#include <Tunic/Application.hpp>
#include <Tunic/ECS/Core/Manager.hpp>
#include <Tunic/ECS/3D/Components/TransformComponent.hpp>
#include <Tunic/ECS/3D/Components/MeshComponent.hpp>
#include <Clove/Graphics/Core/Renderables/Mesh.hpp>
#include <Clove/Graphics/Core/Material.hpp>

namespace Bulb::Core{
	Entity::Entity(tnc::ecs::EntityID ID){
		this->ID = ID;
		components = gcnew System::Collections::Generic::List<Component^>();
	}

	System::Collections::Generic::List<Component^>^ Entity::getComponents(){
		return components;
	}

	void Entity::addTransformComponent(){
		tnc::Application::get().getManager().addComponent<tnc::ecs::_3D::TransformComponent>(ID);
		auto comp = gcnew Component();
		comp->name = "Transform Component";
		components->Add(comp);
	}

	void Entity::addMeshComponent(){
		auto material = std::make_shared<clv::gfx::Material>();
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", material->createInstance());
		tnc::Application::get().getManager().addComponent<tnc::ecs::_3D::MeshComponent>(ID, mesh);
		auto comp = gcnew Component();
		comp->name = "Mesh Component";
		components->Add(comp);
	}
}