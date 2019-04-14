#pragma once

#include "Clove/Rendering/API/Renderable.hpp"

namespace clv{
	class Mesh : public Renderable{
		//VARIABLES
	private:
		std::string meshPath;

		//FUNCTIONS
	public:
		CLV_API Mesh();
		CLV_API Mesh(const std::string& meshPath);
		CLV_API Mesh(const std::string& meshPath, std::shared_ptr<Material> material);
		CLV_API Mesh(const Mesh& other);
		CLV_API Mesh(Mesh&& other) noexcept;

		CLV_API ~Mesh();

		CLV_API void setModelMatrix(const math::Matrix4f& model);

		CLV_API Mesh& operator=(const Mesh& other);
		CLV_API Mesh& operator=(Mesh&& other) noexcept;

	private:
		void createModelData(const std::string& meshPath);
	};
}