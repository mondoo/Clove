#include "clvpch.hpp"
#include "RenderSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Material.hpp"

namespace clv::ecs::d3{
	struct VertexData{
		math::Matrix4f model;
		math::Matrix4f normalMatrix;
	};

	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			TransformComponent* transform = std::get<TransformComponent*>(componentTuple);
			RenderableComponent* renderable = std::get<RenderableComponent*>(componentTuple);

			const math::Matrix4f model = transform->getWorldTransformMatrix();
			renderable->mesh->getMaterial().setData(gfx::BBP_ModelData, VertexData{ model, math::transpose(math::inverse(model)) }, gfx::ShaderType::Vertex);

			gfx::Renderer::submitMesh(renderable->mesh);
		}
	}
}