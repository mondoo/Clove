#pragma once

#include "Clove/ECS/Components/RenderableComponent.hpp"

namespace clv::gfx{
	class Renderer;
	class Shader;
	class Texture;
	struct Vertex;

	class Mesh : public ecs::RenderableComponent{ //TODO: do i want this? I guess just call it a mesh component and slam it in the ECS
		//VARIABLES
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Shader* shader = nullptr;

		//FUNCTIONS
	public:
		Mesh();
		Mesh(const Mesh& other) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(const Mesh& other) = delete;
		Mesh& operator=(Mesh&& other) noexcept;
		virtual ~Mesh();

		void setModelMatrix(const math::Matrix4f& model);

		void setDiffuseTexture(const std::string& path);
		void setSpecularTexture(const std::string& path);
		//void setTexture(std::unique_ptr<Texture> texture);
	};
}