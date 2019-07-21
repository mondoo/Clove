#include "clvpch.hpp"
#include "Render2DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::ecs{
	Render2DSystem::Render2DSystem() = default;

	Render2DSystem::Render2DSystem(Render2DSystem&& other) noexcept = default;

	Render2DSystem& Render2DSystem::operator=(Render2DSystem&& other) noexcept = default;

	Render2DSystem::~Render2DSystem() = default;

	void Render2DSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			Transform2DComponent* transform = std::get<Transform2DComponent*>(componentTuple);
			SpriteComponent* sprite = std::get<SpriteComponent*>(componentTuple);

			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), math::Vector3f(transform->getLocalPosition(), 0.0f));
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), math::Vector3f(transform->getLocalScale(), 0.0f));
			const math::Matrix4f model = translation * scale;

			sprite->submissionData.modelData = model;

			gfx::Renderer::submitSprite(sprite->submissionData);
		}
	}
}