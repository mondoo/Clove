#pragma once

namespace clv::gfx{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;
	
	class Mesh{
		//VARIABLES
	private:
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		/*std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;*/

		//FUNCTIONS
	public:
		Mesh();
		Mesh(const Mesh& other);
		Mesh& operator=(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		Mesh(std::string filePath);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);

		void bind();
	};
}
