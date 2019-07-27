#include "clvpch.hpp"
#include "Render3DSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::ecs{
	Render3DSystem::Render3DSystem() = default;

	Render3DSystem::Render3DSystem(Render3DSystem&& other) noexcept = default;

	Render3DSystem& Render3DSystem::operator=(Render3DSystem&& other) noexcept = default;

	Render3DSystem::~Render3DSystem() = default;

	void Render3DSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			MeshComponent* renderable = std::get<MeshComponent*>(componentTuple);

			const auto& [rot, angle] = transform->getRotation();
			math::Matrix4f translation = math::translate(math::Matrix4f(1.0f), transform->getPosition());
			math::Matrix4f rotation = math::rotate(math::Matrix4f(1.0f), angle, rot);
			math::Matrix4f scale = math::scale(math::Matrix4f(1.0f), transform->getScale());

			renderable->submissionData.modelData = translation * rotation * scale;

			gfx::Renderer::submitMesh(renderable->submissionData);
		}
	}
}