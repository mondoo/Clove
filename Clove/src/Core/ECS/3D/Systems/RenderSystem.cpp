#include "RenderSystem.hpp"

#include "Core/Graphics/Renderer.hpp"
#include "Core/Graphics/ShaderBufferTypes.hpp"
#include "Core/Graphics/Mesh.hpp"
#include "Core/Graphics/MaterialInstance.hpp"

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