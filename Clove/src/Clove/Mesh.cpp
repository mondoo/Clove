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
		CLV_WARN("Copy constructor called for Mesh - creating new model data");
		
		if(other.material){
			material = other.material;
		} else{
			material.reset();
		}
		createModelData(other.meshPath);
	}

	Mesh::Mesh(Mesh&& other){
		va = std::move(other.va);
		vb = std::move(other.vb);
		ib = std::move(other.ib);
		material = std::move(other.material);

		vertexData = std::move(other.vertexData);
		indices = std::move(other.indices);
	}

	Mesh::~Mesh() = default;

	void Mesh::setMVP(const math::Matrix4f& model, const math::Matrix4f& view, const math::Matrix4f& projection){
		material->bindShader();
		material->setUniform4m("model", model);
		material->setUniform4m("view", view);
		material->setUniform4m("projection", projection);
		material->unbindShader();
	}

	void Mesh::draw(const Renderer& renderer){
		material->bindTexture();//TODO: move into renderer?
		renderer.draw(*va, *ib, material->getShaderData());
		material->unbindTexture();
	}

	Mesh& Mesh::operator=(const Mesh& other){
		CLV_WARN("Copy assignment operator called for Mesh - creating new model data");
		if(other.material){
			material = std::make_unique<Material>(*other.material);
		} else{
			material.reset();
		}
		createModelData(other.meshPath);

		return *this;
	}

	Mesh& Mesh::operator=(Mesh&& other){
		va = std::move(other.va);
		vb = std::move(other.vb);
		ib = std::move(other.ib);
		material = std::move(other.material);

		vertexData = std::move(other.vertexData);
		indices = std::move(other.indices);

		return *this;
	}

	std::pair<const void*, unsigned int> Mesh::getVertexData() const{
		return std::pair<const void*, unsigned int>(vertexData.data(), (vertexData.size() * sizeof(float)));
	}

	std::pair<const unsigned int*, unsigned int> Mesh::getIndexData() const{
		return std::pair<const unsigned int*, unsigned int>(indices.data(), indices.size());
	}

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

			auto[vdata, vsize] = getVertexData();
			vb = std::make_unique<VertexBuffer>(VertexBuffer(vdata, vsize));

			va = std::make_unique<VertexArray>(VertexArray());
			va->addBuffer(*vb, layout);

			auto[idata, icount] = getIndexData();
			ib = std::make_unique<IndexBuffer>(IndexBuffer(idata, icount));

			va->unbind();
			vb->unbind();
			ib->unbind();
		} else{
			CLV_ERROR("Could not load mesh for model");
		}
	}
}