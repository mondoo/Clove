#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs::d3{
	class RenderableComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x7dfb2c10; //VS Generated GUID

	private:
		gfx::MeshRenderData submissionData;

		//FUNCTIONS
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent& other) = delete;
		RenderableComponent(RenderableComponent&& other) noexcept;
		RenderableComponent& operator=(const RenderableComponent& other) = delete;
		RenderableComponent& operator=(RenderableComponent&& other) noexcept;
		virtual ~RenderableComponent();

		void setMesh(const std::string& filePath);

		void setAlbedoTexture(const std::string& path);
		void setSpecularTexture(const std::string& path);
	};
}