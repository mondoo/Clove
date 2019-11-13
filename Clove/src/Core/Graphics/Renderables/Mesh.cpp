#include "Mesh.hpp"

#include "Core/Graphics/RenderCommand.hpp"
#include "Core/Graphics/Resources/Buffer.hpp"
#include "Core/Utils/MeshLoader.hpp"

namespace clv::gfx{
	Mesh::Mesh(std::string filePath, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		, loadedBufferData(VertexLayout{}){//Note initialising it like this is potentially dangerous
		loader::MeshInfo info = loader::MeshLoader::loadOBJ(filePath);

		const int32 vertexCount = info.verticies.size();
		indices = info.indices;

		VertexLayout layout; //Layout is currently all possible data
		layout.add(gfx::VertexElementType::position3D).add(VertexElementType::texture2D).add(VertexElementType::normal);
		
		loadedBufferData = { layout };
		loadedBufferData.resize(vertexCount);

		for(int32 i = 0; i < vertexCount; ++i){
			for(int32 j = 0; j < layout.count(); ++j){
				switch(layout.resolve(j).getType()){
					case VertexElementType::position3D:
						loadedBufferData[i].getAttribute<VertexElementType::position3D>() = math::Vector3f{ info.getData<VertexElementType::position3D>()[i] };
						break;

					case VertexElementType::texture2D:
						loadedBufferData[i].getAttribute<VertexElementType::texture2D>() = math::Vector2f{ info.getData<VertexElementType::texture2D>()[i] };
						break;

					case VertexElementType::normal:
						loadedBufferData[i].getAttribute<VertexElementType::normal>() = math::Vector3f{ info.getData<VertexElementType::normal>()[i] };
						break;
					default:
						break;
				}
			}
		}

		initialiseIndexBuffer(indices);
	}

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		, loadedBufferData(vbData)
		, indices(indices){
		initialiseIndexBuffer(indices);
	}

	Mesh::Mesh(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;

	Mesh::~Mesh() = default;

	MaterialInstance& Mesh::getMaterialInstance(){
		return materialInstance;
	}

	uint32 Mesh::getIndexCount(){
		return static_cast<uint32>(indices.size());
	}

	/*void Mesh::bind(){
		RenderCommand::bindVertexBuffer(*vertexBuffer);
		RenderCommand::bindIndexBuffer(*indexBuffer);
		materialInstance.bind();
	}*/

	std::shared_ptr<Buffer> Mesh::generateVertexBuffer(const VertexLayout& layout){
		const int32 vertexCount = loadedBufferData.size();

		gfx::VertexBufferData vertexArray{ layout };
		vertexArray.resize(vertexCount);

		for(int32 i = 0; i < vertexCount; ++i){
			for(int32 j = 0; j < layout.count(); ++j){
				switch(layout.resolve(j).getType()){
					case VertexElementType::position2D:
						vertexArray[i].getAttribute<VertexElementType::position2D>() = loadedBufferData[i].getAttribute<VertexElementType::position2D>();
						break;

					case VertexElementType::position3D:
						vertexArray[i].getAttribute<VertexElementType::position3D>() = loadedBufferData[i].getAttribute<VertexElementType::position3D>();
						break;

					case VertexElementType::texture2D:
						vertexArray[i].getAttribute<VertexElementType::texture2D>() = loadedBufferData[i].getAttribute<VertexElementType::texture2D>();
						break;

					case VertexElementType::normal:
						vertexArray[i].getAttribute<VertexElementType::normal>() = loadedBufferData[i].getAttribute<VertexElementType::normal>();
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

	std::shared_ptr<Buffer> Mesh::getIndexBuffer(){
		return indexBuffer;
	}

	void Mesh::initialiseIndexBuffer(const std::vector<uint32>& indices){
		const std::size_t indexSize = sizeof(uint32);

		BufferDescriptor ibdesc{};
		ibdesc.elementSize	= indexSize;
		ibdesc.bufferSize	= indices.size() * indexSize;
		ibdesc.bufferType	= BufferType::IndexBuffer;
		ibdesc.bufferUsage	= BufferUsage::Default;
		indexBuffer			= RenderCommand::createBuffer(ibdesc, indices.data());
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