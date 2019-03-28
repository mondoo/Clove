#include "clvpch.h"
#include "Model.h"
#include "Clove/MeshLoader.h"
#include "Clove/Rendering/Renderer.h"
//#include "Clove/Rendering/API/VertexBuffer.h"
//#include "Clove/Rendering/API/IndexBuffer.h"
//#include "Clove/Rendering/API/VertexArray.h"
//#include "Clove/Rendering/API/VertexBufferLayout.h"
//#include "Clove/Rendering/API/Shader.h"
//#include "Clove/Rendering/API/Texture.h"

#include <fstream>
#include <strstream>

namespace clv{
	Model::Model(const std::string& mesh){
		createModelData(mesh, "../Clove/res/Textures/DefaultTexture.png");
	}

	Model::Model(const std::string& mesh, const std::string& texture){
		createModelData(mesh, texture);
	}

	Model::Model(Model&& other){
		va = std::move(other.va);
		vb = std::move(other.vb);
		ib = std::move(other.ib);
		shader = std::move(other.shader);
		texture = std::move(other.texture);

		vertexData = other.vertexData;
		indices = other.indices;
	}

	void Model::setMVP(const glm::mat4& MVP){
		shader->bind();
		shader->setUniformMat4f("u_MVP", MVP);
	}

	void Model::draw(const Renderer& renderer){
		const unsigned int slot = 0;
		texture->bind(slot);
		shader->setUniform1i("u_Texture", slot);
		
		renderer.draw(*va, *ib, *shader);
	}

	Model& Model::operator =(Model&& other){
		va = std::move(other.va);
		vb = std::move(other.vb);
		ib = std::move(other.ib);
		shader = std::move(other.shader);
		texture = std::move(other.texture);

		vertexData = other.vertexData;
		indices = other.indices;

		return *this;
	}

	std::pair<const void*, unsigned int> Model::getVertexData() const{
		return std::pair<const void*, unsigned int>(vertexData.data(), (vertexData.size() * sizeof(float)));
	}

	std::pair<const unsigned int*, unsigned int> Model::getIndexData() const{
		return std::pair<const unsigned int*, unsigned int>(indices.data(), indices.size());
	}

	void Model::createModelData(const std::string& meshPath, const std::string& texturePath){
		MeshInfo loadedMeshInfo;
		if(loadOBJ(meshPath, loadedMeshInfo)){
			const int vertexCount = loadedMeshInfo.verticies.size();
			const int texCoordCount = loadedMeshInfo.texCoords.size();
			const int normalCount = loadedMeshInfo.normals.size();

			VertexBufferLayout layout;

			if(vertexCount > 0){
				layout.push<float>(3);
			}

			if(texCoordCount > 0){
				layout.push<float>(2);
			}

			if(normalCount > 0){
				layout.push<float>(3);
			}

			for(int i = 0; i < vertexCount; ++i){
				if(vertexCount > 0){
					vertexData.push_back(loadedMeshInfo.verticies[i].x);
					vertexData.push_back(loadedMeshInfo.verticies[i].y);
					vertexData.push_back(loadedMeshInfo.verticies[i].z);
				}

				if(texCoordCount > 0){
					vertexData.push_back(loadedMeshInfo.texCoords[i].x);
					vertexData.push_back(loadedMeshInfo.texCoords[i].y);
				}

				if(normalCount > 0){
					vertexData.push_back(loadedMeshInfo.normals[i].x);
					vertexData.push_back(loadedMeshInfo.normals[i].y);
					vertexData.push_back(loadedMeshInfo.normals[i].z);
				}
			}
			indices = loadedMeshInfo.indices;

			auto[vdata, vsize] = getVertexData();
			vb = std::unique_ptr<VertexBuffer, VBDeleter>(new VertexBuffer(vdata, vsize));

			va = std::unique_ptr<VertexArray, VADeleter>(new VertexArray());
			va->addBuffer(*vb, layout);

			auto[idata, icount] = getIndexData();
			ib = std::unique_ptr<IndexBuffer, IBDeleter>(new IndexBuffer(idata, icount));

			//Shaders
			shader = std::unique_ptr<Shader, ShaderDeleter>(new Shader("../Clove/res/Shaders/Basic.shader"));
			shader->bind();
			shader->setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

			texture = std::unique_ptr<Texture, TextureDeleter>(new Texture(texturePath));

			va->unbind();
			vb->unbind();
			ib->unbind();
			shader->unbind();
		} else{
			CLV_ERROR("Could not load mesh for model");
		}
	}
}