#include "clvpch.hpp"
#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindable.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	void RenderSystem::update(float deltaTime){
		for(auto& compTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(compTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(compTuple);

			const auto& [rot, angle] = transform->rotation;

			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), transform->position);
			math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, rot);
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), transform->scale);

			math::Matrix4f transformMat = translation * rotation * scale;

			renderable->shader->setModelMatrix(transformMat);

			for(const auto& bindable : renderable->bindables){
				bindable->bind(*renderer);
			}

			renderer->drawIndexed(renderable->indexBuffer->getIndexCount());
		}

		renderer->swapBuffers(); //TODO: Swap here?
	}
}