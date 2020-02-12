#include "Tunic/Rendering/Renderables/Mesh.hpp"

#include "Tunic/Application.hpp"

#include "Clove/Graphics/Core/Resources/Buffer.hpp"
#include "Clove/Graphics/Core/GraphicsFactory.hpp"
#include "Clove/Core/Utils/MeshLoader.hpp"
#include "Clove/Graphics/Core/CommandBuffer.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::rnd{
	Mesh::Mesh(std::string filePath, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		, loadedBufferData(VertexLayout{}){//NOTE: initialising it like this is potentially dangerous
		loader::MeshInfo info = loader::MeshLoader::loadOBJ(filePath);

		const std::size_t vertexCount = info.verticies.size();
		indices = info.indices;

		VertexLayout layout; //Layout should be all possible data a mesh could have (biggest size)
		layout.add(VertexElementType::position3D).add(VertexElementType::texture2D).add(VertexElementType::normal);
		
		loadedBufferData = { layout };
		loadedBufferData.resize(vertexCount);

		for(int32_t i = 0; i < vertexCount; ++i){
			for(int32_t j = 0; j < layout.count(); ++j){
				switch(layout.resolve(j).getType()){
					case VertexElementType::position3D:
						loadedBufferData[i].getAttribute<VertexElementType::position3D>() = mth::vec3f{ info.getData<VertexElementType::position3D>()[i] };
						break;

					case VertexElementType::texture2D:
						loadedBufferData[i].getAttribute<VertexElementType::texture2D>() = mth::vec2f{ info.getData<VertexElementType::texture2D>()[i] };
						break;

					case VertexElementType::normal:
						loadedBufferData[i].getAttribute<VertexElementType::normal>() = mth::vec3f{ info.getData<VertexElementType::normal>()[i] };
						break;
					default:
						break;
				}
			}
		}
		initialiseVertexBuffer(loadedBufferData);
		initialiseIndexBuffer(indices);
	}

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32_t>& indices, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		, loadedBufferData(vbData)
		, indices(indices){
		initialiseVertexBuffer(vbData);
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

	uint32_t Mesh::getIndexCount(){
		return static_cast<uint32_t>(indices.size());
	}

	void Mesh::bind(CommandBuffer& commandBuffer, const VertexLayout& layout){
		const size_t vertexCount = loadedBufferData.size();
		gfx::VertexBufferData vertexArray{ layout };
		vertexArray.resize(vertexCount);

		for (int32_t i = 0; i < vertexCount; ++i){
			for (int32_t j = 0; j < layout.count(); ++j){
				switch (layout.resolve(j).getType()){
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

		commandBuffer.updateBufferData(*vertexBuffer, vertexArray.data());

		commandBuffer.bindVertexBuffer(*vertexBuffer, layout.size());
		commandBuffer.bindIndexBuffer(*indexBuffer);
	}

	void Mesh::initialiseVertexBuffer(const VertexBufferData& vertexArray){
		BufferDescriptor vbdesc{};
		vbdesc.elementSize	= vertexArray.getLayout().size();
		vbdesc.bufferSize	= vertexArray.sizeBytes();
		vbdesc.bufferType	= BufferType::VertexBuffer;
		vbdesc.bufferUsage	= BufferUsage::Dynamic; //Setting it to dynamic here (see TODO in Mesh::getVertexBufferForLayout)
		vertexBuffer = Application::get().getGraphicsFactory().createBuffer(vbdesc, vertexArray.data());
	}

	void Mesh::initialiseIndexBuffer(const std::vector<uint32_t>& indices){
		const std::size_t indexSize = sizeof(uint32_t);

		BufferDescriptor ibdesc{};
		ibdesc.elementSize	= indexSize;
		ibdesc.bufferSize	= indices.size() * indexSize;
		ibdesc.bufferType	= BufferType::IndexBuffer;
		ibdesc.bufferUsage	= BufferUsage::Default;
		indexBuffer = Application::get().getGraphicsFactory().createBuffer(ibdesc, indices.data());
	}
}