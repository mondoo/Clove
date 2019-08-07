#include "clvpch.hpp"
#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::ecs::d3{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			renderable->submissionData.modelData = transform->getWorldTransformMatrix();

			/*
			this will end up like:
			renderable->material->setData(BBP_ModelData, transform->getWorldTransformMatrix());
			*/

			gfx::Renderer::submitMesh(renderable->submissionData);
		}
	}
}