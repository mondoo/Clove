#pragma once

#include "Clove/Graphics/Drawable.hpp"

namespace clv::gfx{
	class Renderer;
	class Shader;
	class Texture;

	class Mesh : public Drawable{
		//VARIABLES
	private:
		std::vector<float> vertices;
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

		void setWorldMatrix(const math::Matrix4f& world);
		void setViewMatrix(const math::Matrix4f& view);
		void setProjection(const math::Matrix4f& projection);

		void setTexture(const std::string& path);
		void setTexture(std::unique_ptr<Texture> texture);
	};
}