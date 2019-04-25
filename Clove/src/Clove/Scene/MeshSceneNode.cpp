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
			this->meshPath = meshPath;

			const loader::MeshInfo& loadedMeshInfo = loader::MeshLoader::loadOBJ(meshPath);

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

			const void* vdata = loadedMeshInfo.vertexData.data();
			unsigned int vsize = static_cast<unsigned int>((loadedMeshInfo.vertexData.size() * sizeof(float)));
			vertexBuffer = std::make_unique<VertexBuffer>(VertexBuffer(vdata, vsize));

			vertexArray = std::make_unique<VertexArray>(VertexArray());
			vertexArray->addBuffer(*vertexBuffer, layout);

			const unsigned int* idata = loadedMeshInfo.indices.data();
			unsigned int icount = static_cast<unsigned int>(loadedMeshInfo.indices.size());
			indexBuffer = std::make_unique<IndexBuffer>(IndexBuffer(idata, icount));

			vertexArray->unbind();
			vertexBuffer->unbind();
			indexBuffer->unbind();
		}
	}
}