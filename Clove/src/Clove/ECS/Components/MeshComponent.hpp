#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs{
	class MeshComponent : public Component{
		friend class Render3DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x7dfb2c10; //VS Generated GUID

	private:
		gfx::MeshRenderData submissionData;

		//FUNCTIONS
	public:
		MeshComponent();
		MeshComponent(const MeshComponent& other) = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent& other) = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;
		virtual ~MeshComponent();

		void setMesh(const std::string& filePath);

		void setDiffuseTexture(const std::string& path);
		void setSpecularTexture(const std::string& path);
	};
}