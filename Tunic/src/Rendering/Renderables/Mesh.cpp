#include "Tunic/Rendering/Renderables/Mesh.hpp"

#include "Tunic/Application.hpp"

#include "Clove/Graphics/Core/Resources/Buffer.hpp"
#include "Clove/Graphics/Core/GraphicsFactory.hpp"
#include "Clove/Graphics/Core/CommandBuffer.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::rnd{
	static VertexBufferData transferVertexBufferData(VertexBufferData& source, const VertexLayout& newLayout){
		const size_t vertexCount = source.size();
		gfx::VertexBufferData vertexArray{ newLayout };
		vertexArray.resize(vertexCount);

		for(int32_t i = 0; i < vertexCount; ++i) {
			for(int32_t j = 0; j < newLayout.count(); ++j) {
				switch(newLayout.resolve(j).getType()) {
					case VertexElementType::position2D:
						vertexArray[i].getAttribute<VertexElementType::position2D>() = source[i].getAttribute<VertexElementType::position2D>();
						break;

					case VertexElementType::position3D:
						vertexArray[i].getAttribute<VertexElementType::position3D>() = source[i].getAttribute<VertexElementType::position3D>();
						break;

					case VertexElementType::texture2D:
						vertexArray[i].getAttribute<VertexElementType::texture2D>() = source[i].getAttribute<VertexElementType::texture2D>();
						break;

					case VertexElementType::normal:
						vertexArray[i].getAttribute<VertexElementType::normal>() = source[i].getAttribute<VertexElementType::normal>();
						break;
					default:
						break;
				}
			}
		}

		return vertexArray;
	}

	static std::shared_ptr<clv::gfx::Buffer> createVertexBuffer(const VertexBufferData& vertexArray) {
		BufferDescriptor vbdesc{};
		vbdesc.elementSize	= vertexArray.getLayout().size();
		vbdesc.bufferSize	= vertexArray.sizeBytes();
		vbdesc.bufferType	= BufferType::VertexBuffer;
		vbdesc.bufferUsage	= BufferUsage::Default;
		auto vertexBuffer	= Application::get().getGraphicsFactory().createBuffer(vbdesc, vertexArray.data());

		return vertexBuffer;
	}

	static std::shared_ptr<clv::gfx::Buffer> createIndexBuffer(const std::vector<uint32_t>& indices) {
		const std::size_t indexSize = sizeof(uint32_t);

		BufferDescriptor ibdesc{};
		ibdesc.elementSize	= indexSize;
		ibdesc.bufferSize	= indices.size() * indexSize;
		ibdesc.bufferType	= BufferType::IndexBuffer;
		ibdesc.bufferUsage	= BufferUsage::Default;
		auto indexBuffer	= Application::get().getGraphicsFactory().createBuffer(ibdesc, indices.data());

		return indexBuffer;
	}

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32_t>& indices, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		, loadedBufferData(vbData)
		, indices(indices) {
		vertexBufferMap[vbData.getLayout()] = createVertexBuffer(vbData);
		indexBuffer = createIndexBuffer(indices);
	}

	Mesh::Mesh(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept
		: materialInstance(std::move(other.materialInstance))
		, loadedBufferData(std::move(other.loadedBufferData)) {
		vertexBufferMap	= std::move(other.vertexBufferMap);
		indexBuffer		= std::move(other.indexBuffer);
		indices			= std::move(other.indices);
	}

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		materialInstance	= std::move(other.materialInstance);
		loadedBufferData	= std::move(other.loadedBufferData);
		vertexBufferMap		= std::move(other.vertexBufferMap);
		indexBuffer			= std::move(other.indexBuffer);
		indices				= std::move(other.indices);

		return *this;
	}

	Mesh::~Mesh() = default;

	void Mesh::setMaterialInstance(MaterialInstance materialInstance) {
		this->materialInstance = materialInstance;
	}

	MaterialInstance& Mesh::getMaterialInstance() {
		return materialInstance;
	}

	uint32_t Mesh::getIndexCount(){
		return static_cast<uint32_t>(indices.size());
	}

	void Mesh::draw(CommandBuffer& commandBuffer, const VertexLayout& layout){
		if(vertexBufferMap.find(layout) == vertexBufferMap.end()) {
			vertexBufferMap[layout] = createVertexBuffer(transferVertexBufferData(loadedBufferData, layout));
		}
		
		commandBuffer.bindVertexBuffer(*vertexBufferMap[layout], layout.size());
		commandBuffer.bindIndexBuffer(*indexBuffer);

		materialInstance.bind(commandBuffer);

		commandBuffer.drawIndexed(getIndexCount());
	}
}