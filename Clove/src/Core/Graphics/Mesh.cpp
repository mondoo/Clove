#include "Mesh.hpp"

#include "Core/Graphics/BindableFactory.hpp"
#include "Core/Graphics/Bindables/VertexBuffer.hpp"
#include "Core/Graphics/Bindables/IndexBuffer.hpp"
#include "Core/Graphics/Material.hpp"
#include "Core/Graphics/Bindables/Shader.hpp"
#include "Core/Graphics/VertexLayout.hpp"
#include "Core/Utils/MeshLoader.hpp"

namespace clv::gfx{
	Mesh::Mesh(const Mesh& other) = default;

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	Mesh::Mesh(std::string filePath, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance)){
		loader::MeshInfo info = loader::MeshLoader::loadOBJ(filePath);

		this->materialInstance.bind();

		const int32 vertexCount = info.verticies.size();

		gfx::VertexLayout layout = this->materialInstance.getReflectionData().vertexBufferLayout;
		gfx::VertexBufferData vertexArray{ layout };
		vertexArray.resize(vertexCount);
		
		for(int32 i = 0; i < vertexCount; ++i){
			for(int32 j = 0; j < layout.count(); ++j){
				switch(layout.resolve(j).getType()){
					case VertexElementType::position3D:
						vertexArray[i].getAttribute<VertexElementType::position3D>() = math::Vector3f{ info.getData<VertexElementType::position3D>()[i] };
						break;

					case VertexElementType::texture2D:
						vertexArray[i].getAttribute<VertexElementType::texture2D>() = math::Vector2f{ info.getData<VertexElementType::texture2D>()[i] };
						break;

					case VertexElementType::normal:
						vertexArray[i].getAttribute<VertexElementType::normal>() = math::Vector3f{ info.getData<VertexElementType::normal>()[i] };
						break;
					default:
						break;
				}
			}
		}

		//VB
		vertexBuffer = gfx::BindableFactory::createVertexBuffer(vertexArray, *this->materialInstance.getShader());

		//IB
		indexBuffer = gfx::BindableFactory::createIndexBuffer(info.indices);
	}

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance)){
		
		this->materialInstance.bind();

		//VB
		vertexBuffer = gfx::BindableFactory::createVertexBuffer(vbData, *this->materialInstance.getShader());

		//IB
		indexBuffer = gfx::BindableFactory::createIndexBuffer(indices);
	}

	MaterialInstance& Mesh::getMaterialInstance(){
		return materialInstance;
	}

	uint32 Mesh::getIndexCount(){
		return indexBuffer->getIndexCount();
	}

	void Mesh::bind(){
		vertexBuffer->bind();
		indexBuffer->bind();
		materialInstance.bind();
	}
}