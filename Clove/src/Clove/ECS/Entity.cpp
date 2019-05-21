#include "clvpch.hpp"
#include "Entity.hpp"

#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Scene/Camera.hpp"

namespace clv::ecs{
	Entity::Entity()
		: scene::SceneNode(){
		mesh = std::make_shared<gfx::Mesh>(Application::get().getWindow().getRenderer());

		//Temp
		//mesh->setViewMatrix(math::Matrix4f(1.0f));
		//mesh->setProjection(math::createPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.5f, 50.0f));
	}

	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept = default;

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	void Entity::update(float deltaSeconds){
		scene::SceneNode::update(deltaSeconds);

		mesh->setWorldMatrix(getWorldTransform());

		gfx::Renderer& renderer = Application::get().getWindow().getRenderer();
		mesh->setViewMatrix(renderer.getActiveCamera().getWorldTransform());
		mesh->setProjection(renderer.getActiveCamera().getProjection());

		renderer.submit(mesh);
	}
}
