#include "clvpch.hpp"
#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindable.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(float deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			const auto& [rot, angle] = transform->rotation;

			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), transform->position);
			math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, rot);
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), transform->scale);

			math::Matrix4f transformMat = translation * rotation * scale;

			//TODO: need a way to parent transforms together

			renderable->shader->setModelMatrix(transformMat);

			for(const auto& bindable : renderable->bindables){
				bindable->bind(*renderer);
			}

			renderer->drawIndexed(renderable->indexBuffer->getIndexCount());
		}

		renderer->swapBuffers();
	}
}