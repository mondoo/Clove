#pragma once

namespace clv{
	class Renderer;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Material;

	class Mesh{
		//VARIABLES
	private:
		std::vector<float> vertexData;
		std::vector<unsigned int> indices;

		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;

		std::shared_ptr<Material> material;

		std::string meshPath;

		//FUNCTIONS
	public:
		CLV_API Mesh();
		CLV_API Mesh(const std::string& meshPath);
		CLV_API Mesh(const std::string& meshPath, std::shared_ptr<Material> material);
		CLV_API Mesh(const Mesh& other);
		CLV_API Mesh(Mesh&& other) noexcept;

		CLV_API ~Mesh();

		CLV_API void setMVP(const math::Matrix4f& model, const math::Matrix4f& view, const math::Matrix4f& projection);

		CLV_API void draw(const Renderer& renderer);

		CLV_API Mesh& operator=(const Mesh& other);
		CLV_API Mesh& operator=(Mesh&& other) noexcept;

	private:
		std::pair<const void*, unsigned int> getVertexData() const;
		std::pair<const unsigned int*, unsigned int> getIndexData() const;

		void createModelData(const std::string& meshPath);
	};
}