#include "clvpch.h"
#include "Mesh.h"
#include "Clove/MeshLoader.h"
#include "Clove/Rendering/Renderer.h"
#include "Clove/Rendering/API/VertexBuffer.h"
#include "Clove/Rendering/API/IndexBuffer.h"
#include "Clove/Rendering/API/VertexArray.h"
#include "Clove/Rendering/API/VertexBufferLayout.h"
#include "Clove/Rendering/API/Shader.h"
#include "Clove/Rendering/API/Texture.h"

#include <fstream>

namespace clv{
	Mesh::Mesh() = default;

	Mesh::Mesh(const std::string& meshPath){
		createModelData(meshPath, "../Clove/res/Textures/DefaultTexture.png");
	}

	Mesh::Mesh(const std::string& meshPath, const std::string& texturePath){
		createModelData(meshPath, texturePath);
	}

	Mesh::Mesh(const Mesh& other){
		CLV_WARN("Copy constructor called for Mesh - creating new model data");
		createModelData(other.meshPath, other.texturePath);
	}

	Mesh::Mesh(Mesh&& other){
		va = std::move(other.va);
		vb = std::move(other.vb);
		ib = std::move(other.ib);
		shader = std::move(other.shader);
		texture = std::move(other.texture);

		vertexData = other.vertexData;
		indices = other.indices;
	}

	Mesh::~Mesh() = default;

	void Mesh::setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection){
		shader->bind();
		shader->setUniformMat4f("model", model);
		shader->setUniformMat4f("view", view);
		shader->setUniformMat4f("projection", projection);
	}

	void Mesh::setAmbientStrength(float strength){
		ambientStrength = strength;
	}

	void Mesh::setLightPosition(const glm::vec3 &pos){
		lightPosition = pos;
	}

	void Mesh::setViewPosition(const glm::vec3& pos){
		viewPosition = pos;
	}

	void Mesh::draw(const Renderer& renderer){
		const unsigned int slot = 0;
		texture->bind(slot);
		shader->setUniform1i("u_Texture", slot);

		shader->setUniform1f("ambientStrength", ambientStrength);
		shader->setUniform3f("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
		shader->setUniform3f("viewPos", viewPosition.x, viewPosition.y, viewPosition.z);
		
		renderer.draw(*va, *ib, *shader);
	}

	Mesh& Mesh::operator=(const Mesh& other){
		CLV_WARN("Copy assignment operator called for Mesh - creating new model data");
		createModelData(other.meshPath, other.texturePath);

		return *this;
	}

	Mesh& Mesh::operator=(Mesh&& other)
	{
		va = std::move(other.va);
		vb = std::move(other.vb);
		ib = std::move(other.ib);
		shader = std::move(other.shader);
		texture = std::move(other.texture);

		vertexData = other.vertexData;
		indices = other.indices;

		return *this;
	}

	std::pair<const void*, unsigned int> Mesh::getVertexData() const{
		return std::pair<const void*, unsigned int>(vertexData.data(), (vertexData.size() * sizeof(float)));
	}

	std::pair<const unsigned int*, unsigned int> Mesh::getIndexData() const{
		return std::pair<const unsigned int*, unsigned int>(indices.data(), indices.size());
	}

	void Mesh::createModelData(const std::string& meshPath, const std::string& texturePath){
		CLV_TRACE("Creating model with: {0} | {1}", meshPath, texturePath);

		this->meshPath = meshPath;
		this->texturePath = texturePath;

		MeshInfo loadedMeshInfo;
		if(loadOBJ(meshPath, loadedMeshInfo)){
			const size_t vertexCount = loadedMeshInfo.verticies.size();
			const size_t texCoordCount = loadedMeshInfo.texCoords.size();
			const size_t normalCount = loadedMeshInfo.normals.size();

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
			vb = std::make_unique<VertexBuffer>(VertexBuffer(vdata, vsize));

			va = std::make_unique<VertexArray>(VertexArray());
			va->addBuffer(*vb, layout);

			auto[idata, icount] = getIndexData();
			ib = std::make_unique<IndexBuffer>(IndexBuffer(idata, icount));

			//Shaders
			shader = std::make_unique<Shader>(Shader(/*"../Clove/res/Shaders/BasicShader.glsl"*/));

			shader->attachShader(ShaderTypes::Vertex, "../Clove/res/Shaders/VertexShader.glsl");
			shader->attachShader(ShaderTypes::Fragment, "../Clove/res/Shaders/FragmentShader.glsl");

			shader->bind();
			shader->setUniform4f("objectColour", 0.65f, 0.65f, 0.65f, 1.0f);
			shader->setUniform4f("lightColour", 1.0f, 1.0f, 1.0f, 1.0f);

			texture = std::make_unique<Texture>(Texture(texturePath));

			va->unbind();
			vb->unbind();
			ib->unbind();
			shader->unbind();
		} else{
			CLV_ERROR("Could not load mesh for model");
		}
	}
}