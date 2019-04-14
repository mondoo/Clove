#include "clvpch.hpp"
#include "Mesh.hpp"

#include "Clove/MeshLoader.hpp"
#include "Clove/Rendering/Renderer.hpp"
#include "Clove/Rendering/API/VertexBuffer.hpp"
#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/VertexBufferLayout.hpp"
#include "Clove/Rendering/API/Material.hpp"

#include <fstream>

namespace clv{
	Mesh::Mesh() = default;

	Mesh::Mesh(const std::string& meshPath){
		material = std::make_shared<Material>(Material());
		createModelData(meshPath);
	}

	Mesh::Mesh(const std::string& meshPath, std::shared_ptr<Material> material){
		this->material = material;
		createModelData(meshPath);
	}

	Mesh::Mesh(const Mesh& other){
		CLV_WARN("Copy constructor called on Mesh - creating new data");
		material = other.material;
		createModelData(other.meshPath);
	}

	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	void Mesh::setModelMatrix(const math::Matrix4f& model){
		material->setUniform("model", model);
	}

	Mesh& Mesh::operator=(const Mesh& other){
		CLV_WARN("Copy assignment operator called on Mesh - creating new data");
		material = other.material;
		createModelData(other.meshPath);

		return *this;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	void Mesh::createModelData(const std::string& meshPath){
		CLV_TRACE("Creating model with: {0}", meshPath);

		this->meshPath = meshPath;

		vertexData.clear();
		indices.clear();

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

			vertexData.reserve((vertexCount * 3) + (texCoordCount * 2) + (normalCount * 3));

			for(int i = 0; i < vertexCount; ++i){
				if(vertexCount > 0){
					vertexData.emplace_back(loadedMeshInfo.verticies[i].x);
					vertexData.emplace_back(loadedMeshInfo.verticies[i].y);
					vertexData.emplace_back(loadedMeshInfo.verticies[i].z);
				}

				if(texCoordCount > 0){
					vertexData.emplace_back(loadedMeshInfo.texCoords[i].x);
					vertexData.emplace_back(loadedMeshInfo.texCoords[i].y);
				}

				if(normalCount > 0){
					vertexData.emplace_back(loadedMeshInfo.normals[i].x);
					vertexData.emplace_back(loadedMeshInfo.normals[i].y);
					vertexData.emplace_back(loadedMeshInfo.normals[i].z);
				}
			}
			indices = loadedMeshInfo.indices;

			const void* vdata = vertexData.data();
			unsigned int vsize = (vertexData.size() * sizeof(float));
			vertexBuffer = std::make_unique<VertexBuffer>(VertexBuffer(vdata, vsize));

			vertexArray = std::make_unique<VertexArray>(VertexArray());
			vertexArray->addBuffer(*vertexBuffer, layout);

			const unsigned int* idata = indices.data();
			unsigned int icount = indices.size();
			indexBuffer = std::make_unique<IndexBuffer>(IndexBuffer(idata, icount));

			vertexArray->unbind();
			vertexBuffer->unbind();
			indexBuffer->unbind();
		} else{
			CLV_ERROR("Could not load mesh for model");
		}
	}
}