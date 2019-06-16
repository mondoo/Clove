#pragma once

#include "Clove/ECS/Components/RenderableComponent.hpp"

namespace clv::gfx{
	class Renderer;
	class Shader;
	class Texture;
	struct Vertex;
}

namespace clv::ecs{
	class MeshComponent : public ecs::RenderableComponent{
		//VARIABLES
	private:
		std::vector<gfx::Vertex> vertices;
		std::vector<unsigned int> indices;

		gfx::Shader* shader = nullptr;

		//FUNCTIONS
	public:
		MeshComponent();
		MeshComponent(const MeshComponent& other) = delete;
		MeshComponent(MeshComponent&& other) noexcept;
		MeshComponent& operator=(const MeshComponent& other) = delete;
		MeshComponent& operator=(MeshComponent&& other) noexcept;
		virtual ~MeshComponent();

		void setModelMatrix(const math::Matrix4f& model);

		void setMesh(const std::string& filePath);

		void setDiffuseTexture(const std::string& path);
		void setSpecularTexture(const std::string& path);
	};
}