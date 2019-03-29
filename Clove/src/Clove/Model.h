#pragma once

#include <glm/glm.hpp>

namespace clv{
	class Renderer;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;

	struct MeshInfo;

	class CLV_API Model{
		//VARIABLES
	private:
		bool isUsingTexture = false;

		std::vector<float> vertexData;
		std::vector<unsigned int> indices;

		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

		//FUNCTIONS
	public:
		Model();
		Model(const std::string& mesh);
		Model(const std::string& mesh, const std::string& texture);
		Model(Model&& other);

		~Model();

		//temp ish
		void setMVP(const glm::mat4& MVP);
		//~

		void draw(const Renderer& renderer);

		Model& operator=(Model&& other);

	private:
		std::pair<const void*, unsigned int> getVertexData() const;
		std::pair<const unsigned int*, unsigned int> getIndexData() const;

		void createModelData(const std::string& meshPath, const std::string& texturePath);
	};
}