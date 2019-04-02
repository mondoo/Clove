#pragma once

namespace clv{
	class Renderer;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;

	struct MeshInfo;

	class Model{
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

		//Temp
		float ambientStrength = 1.0f;
		glm::vec3 lightPosition;
		glm::vec3 viewPosition;
		//~

		//FUNCTIONS
	public:
		CLV_API Model();
		CLV_API Model(const std::string& mesh);
		CLV_API Model(const std::string& mesh, const std::string& texture);
		CLV_API Model(Model&& other);

		CLV_API ~Model();

		//temp ish
		CLV_API void setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
		//~

		//temp lighting stuff
		CLV_API void setAmbientStrength(float strength);
		CLV_API void setLightPosition(const glm::vec3& pos);
		CLV_API void setViewPosition(const glm::vec3& pos);
		//~

		CLV_API void draw(const Renderer& renderer);

		CLV_API Model& operator=(Model&& other);

	private:
		std::pair<const void*, unsigned int> getVertexData() const;
		std::pair<const unsigned int*, unsigned int> getIndexData() const;

		void createModelData(const std::string& meshPath, const std::string& texturePath);
	};
}