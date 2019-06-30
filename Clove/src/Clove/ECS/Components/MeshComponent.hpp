#pragma once

#include "Clove/ECS/Components/Renderable3DComponent.hpp"

namespace clv::gfx{
	class Renderer;
	class Shader;
	class Texture;
}

namespace clv::ecs{
	class MeshComponent : public ecs::Renderable3DComponent{
		//VARIABLES
	private:
		std::vector<unsigned int> indices;

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