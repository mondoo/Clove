#include "clvpch.hpp"
#include "MeshSceneNode.hpp"

#include "Clove/MeshLoader.hpp"
#include "Clove/Rendering/Renderer.hpp"
#include "Clove/Rendering/API/VertexBuffer.hpp"
#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/VertexBufferLayout.hpp"
#include "Clove/Rendering/API/Material.hpp"

#include <fstream>

namespace clv{
	namespace scene{
		MeshSceneNode::MeshSceneNode() = default;

		MeshSceneNode::MeshSceneNode(const std::string& meshPath){
			material = std::make_shared<Material>(Material());
			createModelData(meshPath);
		}

		MeshSceneNode::MeshSceneNode(const std::string& meshPath, std::shared_ptr<Material> material){
			this->material = material;
			createModelData(meshPath);
		}

		MeshSceneNode::MeshSceneNode(const MeshSceneNode& other){
			CLV_WARN("Copy constructor called on Mesh - creating new data");
			material = other.material;
			createModelData(other.meshPath);
		}

		MeshSceneNode::MeshSceneNode(MeshSceneNode&& other) noexcept = default;

		MeshSceneNode::~MeshSceneNode() = default;

		void MeshSceneNode::setMesh(const std::string& meshPath){
			createModelData(meshPath);
		}

		MeshSceneNode& MeshSceneNode::operator=(const MeshSceneNode& other){
			CLV_WARN("Copy assignment operator called on Mesh - creating new data");
			material = other.material;
			createModelData(other.meshPath);

			return *this;
		}

		MeshSceneNode& MeshSceneNode::operator=(MeshSceneNode&& other) noexcept = default;

		void MeshSceneNode::createModelData(const std::string& meshPath){
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
				unsigned int vsize = static_cast<unsigned int>((vertexData.size() * sizeof(float)));
				vertexBuffer = std::make_unique<VertexBuffer>(VertexBuffer(vdata, vsize));

				vertexArray = std::make_unique<VertexArray>(VertexArray());
				vertexArray->addBuffer(*vertexBuffer, layout);

				const unsigned int* idata = indices.data();
				unsigned int icount = static_cast<unsigned int>(indices.size());
				indexBuffer = std::make_unique<IndexBuffer>(IndexBuffer(idata, icount));

				vertexArray->unbind();
				vertexBuffer->unbind();
				indexBuffer->unbind();
			} else{
				CLV_ERROR("Could not load mesh for model");
			}
		}
	}
}