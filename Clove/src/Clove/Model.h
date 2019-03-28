#pragma once

#include <glm/glm.hpp>

//TODO: Remove
#include "Clove/Rendering/API/VertexBuffer.h"
#include "Clove/Rendering/API/IndexBuffer.h"
#include "Clove/Rendering/API/VertexArray.h"
#include "Clove/Rendering/API/VertexBufferLayout.h"
#include "Clove/Rendering/API/Shader.h"
#include "Clove/Rendering/API/Texture.h"


namespace clv{
	class Renderer;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;

	class MeshInfo;

	struct VADeleter{
		void operator ()(VertexArray* va){
			va->deleteArray();
		}
	};

	struct VBDeleter{
		void operator ()(VertexBuffer* vb){
			vb->deleteBuffer();
		}
	};

	struct IBDeleter{
		void operator ()(IndexBuffer* ib){
			ib->deleteBuffer();
		}
	};

	struct ShaderDeleter{
		void operator ()(Shader* sh){
			sh->deleteShader();
		}
	};

	struct TextureDeleter{
		void operator ()(Texture* tx){
			tx->deleteTexture();
		}
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

		Model& operator =(Model&& other);

	private:
		std::pair<const void*, unsigned int> getVertexData() const;
		std::pair<const unsigned int*, unsigned int> getIndexData() const;

		void createModelData(const std::string& meshPath, const std::string& texturePath);
	};
}