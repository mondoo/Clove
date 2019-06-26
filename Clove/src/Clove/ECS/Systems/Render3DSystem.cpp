#include "clvpch.hpp"
#include "Render3DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindable.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	Render3DSystem::Render3DSystem() = default;

	Render3DSystem::Render3DSystem(Render3DSystem&& other) noexcept = default;

	Render3DSystem& Render3DSystem::operator=(Render3DSystem&& other) noexcept = default;

	Render3DSystem::~Render3DSystem() = default;

	void Render3DSystem::update(float deltaTime){
		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			Renderable3DComponent* renderable = std::get<Renderable3DComponent*>(componentTuple);

			renderable->shader->setModelMatrix(getTransformWorldMatrix(transform));

			for(const auto& bindable : renderable->bindables){
				bindable->bind(*renderer);
			}

			renderer->drawIndexed(renderable->indexBuffer->getIndexCount());
		}
	}

	void Render3DSystem::initialiseRenderer(const std::shared_ptr<gfx::Renderer>& renderer){
		this->renderer = renderer;
	}

	gfx::Renderer& Render3DSystem::getRenderer(){
		return *renderer;
	}
	math::Matrix4f Render3DSystem::getTransformWorldMatrix(Transform3DComponent* component){
		const auto& [rot, angle] = component->getLocalRotation();

		math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), component->getLocalPosition());
		math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, rot);
		math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), component->getLocalScale());

		math::Matrix4f transform = translation * rotation * scale;

		if(Transform3DComponent* parent = component->getParent()){
			return getTransformWorldMatrix(parent) * transform;
		} else{
			return transform;
		}
	}
}