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

	struct VADeleter{
		void operator()(VertexArray* va);
	};

	struct VBDeleter{
		void operator()(VertexBuffer* vb);
	};

	struct IBDeleter{
		void operator()(IndexBuffer* ib);
	};

	struct ShaderDeleter{
		void operator()(Shader* sh);
	};

	struct TextureDeleter{
		void operator()(Texture* tx);
	};

	class CLV_API Model{
		//VARIABLES
	private:
		bool isUsingTexture = false;

		std::vector<float> vertexData;
		std::vector<unsigned int> indices;

		std::unique_ptr<VertexArray, VADeleter> va;
		std::unique_ptr<VertexBuffer, VBDeleter> vb;
		std::unique_ptr<IndexBuffer, IBDeleter> ib;
		std::unique_ptr<Shader, ShaderDeleter> shader;
		std::unique_ptr<Texture, TextureDeleter> texture;

		//FUNCTIONS
	public:
		Model() = default;
		Model(const std::string& mesh);
		Model(const std::string& mesh, const std::string& texture);
		Model(Model&& other);

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