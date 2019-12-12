#include "Entity.hpp"

#include <Clove/Core/ECS/Entity.hpp>
#include <Clove/Core/ECS/3D/Components/TransformComponent.hpp>
#include <Clove/Core/ECS/3D/Components/MeshComponent.hpp>
#include <Clove/Core/ECS/3D/Components/CameraComponent.hpp>
#include <Clove/Core/Graphics/Renderables/Mesh.hpp>
#include <Clove/Core/Graphics/Material.hpp>
#include <Clove/Core/Graphics/MaterialInstance.hpp>

namespace clv::ecs::blb{
	NativeEntity::NativeEntity(EntityID entityID, Manager* manager)
		: Entity(entityID, manager){
	}

	void NativeEntity::makeCamEnt(int width, int height){
		addComponent<clv::ecs::_3D::TransformComponent>();
		addComponent<clv::ecs::_3D::CameraComponent>(clv::gfx::Viewport{ 0, 0, static_cast<clv::uint32>(width), static_cast<clv::uint32>(height) }); //Any old viewport for now
	}

	void NativeEntity::makeCubeEnt(){
		auto cubeMaterial = std::make_shared<clv::gfx::Material>();
		cubeMaterial->setData(clv::gfx::BBP_Colour, clv::mth::vec4f{ 0.4f, 0.4f, 0.4f, 1.0f }, clv::gfx::ShaderType::Pixel);
		cubeMaterial->setData(clv::gfx::BBP_MaterialData, clv::gfx::MaterialData{ 32.0f }, clv::gfx::ShaderType::Pixel);

		auto inst = cubeMaterial->createInstance();
		auto mesh = std::make_shared<clv::gfx::Mesh>("res/Objects/cube.obj", inst); //NOTE: Manually copied to the correct folder for now

		addComponent<clv::ecs::_3D::TransformComponent>()->setPosition({ 10.0f, 0.0f, 0.0f });
		addComponent<clv::ecs::_3D::MeshComponent>()->setMesh(mesh);
	}
}

namespace Bulb::Core{
	Entity::Entity(clv::ecs::blb::NativeEntity* entity)
		: nativeEntity(entity){
	}

	Entity::!Entity(){
		delete nativeEntity;
	}

	void Entity::makeCamEnt(int width, int height){
		nativeEntity->makeCamEnt(width, height);
	}

	void Entity::makeCubeEnt(){
		nativeEntity->makeCubeEnt();
	}
}