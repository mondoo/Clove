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
		MeshInfo loadedMeshInfo;
		if (loadOBJ(mesh, loadedMeshInfo)){
			//Ignoring norm for now
			const int elementCount = loadedMeshInfo.verticies.size(); //All vectors should be the same size -- add warn or error?

			for(int i = 0; i < elementCount; ++i){
				vertexData.push_back(loadedMeshInfo.verticies[i].x);
				vertexData.push_back(loadedMeshInfo.verticies[i].y);
				vertexData.push_back(loadedMeshInfo.verticies[i].z);

				vertexData.push_back(loadedMeshInfo.texCoords[i].x);
				vertexData.push_back(loadedMeshInfo.texCoords[i].y);
			}
			indices = loadedMeshInfo.indices;

			//Vertex Buffer
			va = std::make_unique<VertexArray>(VertexArray());
			auto[vdata, vsize] = getVertexData();
			vb = std::make_unique<VertexBuffer>(VertexBuffer(vdata, vsize));
			VertexBufferLayout layout;
			layout.push<float>(3); //pos
			layout.push<float>(2); //tex coord
			va->addBuffer(*vb, layout);

			//Index Buffer
			auto[idata, icount] = getIndexData();
			ib = std::make_unique<IndexBuffer>(IndexBuffer(idata, icount));

			//Shaders
			shader = std::make_unique<Shader>(Shader("../Clove/res/Shaders/Basic.shader"));
			shader->bind();
			/*shader.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);*/

			texture = std::make_unique<Texture>(Texture("res/Textures/Zombie-32x32.png"));
			texture->bind();
			shader->setUniform1i("u_Texture", 0);//<- 0 here is what ever slot we bind the texture to

			va->unbind();
			vb->unbind();
			ib->unbind();
			shader->unbind();

		} else{
			CLV_ERROR("Could not load mesh for model");
		}
	}

	Model::Model(const std::string& mesh, const std::string& texture){
		//TODO
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

	Model::~Model(){
		if(va){
			va->deleteArray();
		}
		if(vb){
			vb->deleteBuffer();
		}
		if(ib){
			ib->deleteBuffer();
		}
		if(shader){
			shader->deleteShader();
		}
		if(texture){
			texture->deleteTexture();
		}
	}

	void Model::setMVP(const glm::mat4& MVP){
		shader->bind();
		shader->setUniformMat4f("u_MVP", MVP);
	}

	void Model::draw(const Renderer& renderer){
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
}