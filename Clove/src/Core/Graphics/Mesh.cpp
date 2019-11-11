#include "Mesh.hpp"

#include "Core/Graphics/RenderCommand.hpp"
#include "Core/Graphics/Resources/Buffer.hpp"
#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	Mesh::Mesh(std::string filePath, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance)){
		info = loader::MeshLoader::loadOBJ(filePath);

		//this->materialInstance.bind(); //TODO: Needed?

		/*const int32 vertexCount = info.verticies.size();*/
		indexCount = static_cast<uint32>(info.indices.size());

		//TODO: When / how to do this last bit?
		//Should it be when we prep the mesh for the shader?

		/*gfx::VertexLayout layout = this->materialInstance.getReflectionData().vertexBufferLayout;
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
		}*/

		//createBuffers(vertexArray, info.indices);
	}

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		, indexCount(static_cast<uint32>(indices.size())){
		this->materialInstance.bind(); //TODO: Needed?
	}

	Mesh::Mesh(const Mesh& other) = default;

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	MaterialInstance& Mesh::getMaterialInstance(){
		return materialInstance;
	}

	uint32 Mesh::getIndexCount(){
		return indexCount;
	}

	/*void Mesh::bind(){
		RenderCommand::bindVertexBuffer(*vertexBuffer);
		RenderCommand::bindIndexBuffer(*indexBuffer);
		materialInstance.bind();
	}*/

	std::shared_ptr<Buffer> Mesh::generateVertexBuffer(const VertexLayout& layout){
		const int32 vertexCount = info.verticies.size();

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
		
		BufferDescriptor vbdesc{};
		vbdesc.elementSize	= layout.size();
		vbdesc.bufferSize	= vertexArray.sizeBytes();
		vbdesc.bufferType	= BufferType::VertexBuffer;
		vbdesc.bufferUsage	= BufferUsage::Default;
		auto buffer = RenderCommand::createBuffer(vbdesc, vertexArray.data());

		return buffer;
	}

	std::shared_ptr<Buffer> Mesh::generateIndexBuffer(){
		const std::size_t indexSize = sizeof(uint32);

		BufferDescriptor ibdesc{};
		ibdesc.elementSize	= indexSize;
		ibdesc.bufferSize	= info.indices.size() * indexSize;
		ibdesc.bufferType	= BufferType::IndexBuffer;
		ibdesc.bufferUsage	= BufferUsage::Default;
		auto buffer = RenderCommand::createBuffer(ibdesc, info.indices.data());

		return buffer;
	}

	//void Mesh::createBuffers(const VertexBufferData& vbData, const std::vector<uint32>& indices){
	//	//TODO: Pipeline object??? or somewhere else

	//	//VB
	//	BufferDescriptor vbdesc{};
	//	vbdesc.elementSize	= vbData.getLayout().size();
	//	vbdesc.bufferSize	= vbData.sizeBytes();
	//	vbdesc.bufferType	= BufferType::VertexBuffer;
	//	vbdesc.bufferUsage	= BufferUsage::Default;
	//	vertexBuffer = RenderCommand::createBuffer(vbdesc, vbData.data());

	//	//IB
	//	const std::size_t indexSize = sizeof(uint32);
	//	BufferDescriptor ibdesc{};
	//	ibdesc.elementSize	= indexSize;
	//	ibdesc.bufferSize	= indices.size() * indexSize;
	//	ibdesc.bufferType	= BufferType::IndexBuffer;
	//	ibdesc.bufferUsage	= BufferUsage::Default;
	//	indexBuffer = RenderCommand::createBuffer(ibdesc, indices.data());
	//}
}