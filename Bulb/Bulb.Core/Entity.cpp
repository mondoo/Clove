#include "Entity.hpp"

#include <Clove/Core/Platform/Application.hpp>
#include <Clove/Core/ECS/Manager.hpp>
#include <Clove/Core/ECS/3D/Components/TransformComponent.hpp>
#include <Clove/Core/ECS/3D/Components/MeshComponent.hpp>
#include <Clove/Core/Graphics/Renderables/Mesh.hpp>
#include <Clove/Core/Graphics/Material.hpp>

namespace Bulb::Core{
	Entity::Entity(clv::ecs::EntityID ID){
		this->ID = ID;
		components = gcnew System::Collections::Generic::List<Component^>();
	}

	System::Collections::Generic::List<Component^>^ Entity::getComponents(){
		return components;
	}

	void Entity::addTransformComponent(){
		clv::plt::Application::get().getManager().addComponent<clv::ecs::_3D::TransformComponent>(ID);
		auto comp = gcnew Component();
		comp->name = "Transform Component";
		components->Add(comp);
	}

	void Entity::addMeshComponent(){
		auto material = std::make_shared<clv::gfx::Material>();
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", material->createInstance());
		clv::plt::Application::get().getManager().addComponent<clv::ecs::_3D::MeshComponent>(ID, mesh);
		auto comp = gcnew Component();
		comp->name = "Mesh Component";
		components->Add(comp);
	}
}