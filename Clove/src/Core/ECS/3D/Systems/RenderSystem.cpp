#include "RenderSystem.hpp"

#include "Core/ECS/Manager.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/ECS/3D/Components/MeshComponent.hpp"
#include "Core/Graphics/Renderer.hpp"
#include "Core/Graphics/ShaderBufferTypes.hpp"
#include "Core/Graphics/Renderables/Mesh.hpp"
#include "Core/Graphics/MaterialInstance.hpp"

namespace clv::ecs::_3D{
	RenderSystem::RenderSystem() = default;

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::update(utl::DeltaTime deltaTime){
		auto componentTuples = manager->getComponentSets<TransformComponent, MeshComponent>();
		for(auto& tuple : componentTuples){
			TransformComponent* transform = std::get<TransformComponent*>(tuple);
			MeshComponent* renderable = std::get<MeshComponent*>(tuple);

			const math::Matrix4f model = transform->getWorldTransformMatrix();
			renderable->mesh->getMaterialInstance().setData(gfx::BBP_ModelData, gfx::VertexData{ model, math::transpose(math::inverse(model)) }, gfx::ShaderType::Vertex);

			gfx::Renderer::submitMesh(renderable->mesh);
		}
	}
}