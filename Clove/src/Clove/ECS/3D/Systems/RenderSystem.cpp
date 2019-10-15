#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/MaterialInstance.hpp"

namespace clv::ecs::d3{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			const math::Matrix4f model = transform->getWorldTransformMatrix();
			renderable->mesh->getMaterialInstance().setData(gfx::BBP_ModelData, gfx::VertexData{ model, math::transpose(math::inverse(model)) }, gfx::ShaderType::Vertex);

			gfx::Renderer::submitMesh(renderable->mesh);
		}
	}
}