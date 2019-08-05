#include "clvpch.hpp"
#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::ecs::d2{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		//Submit quad to be used for sprites?

		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			renderable->submissionData.modelData = transform->getWorldTransformMatrix();

			//Do this part

			gfx::Renderer::submitSprite(renderable->submissionData);
		}

		//Finished submitting sprite quads
	}
}