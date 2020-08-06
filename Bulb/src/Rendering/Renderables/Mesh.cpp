#include "Bulb/Rendering/Renderables/Mesh.hpp"

//#include "Clove/Graphics/Buffer.hpp"
#include "Clove/Graphics/GraphicsFactory.hpp"
//#include "Clove/Graphics/CommandBuffer.hpp"

using namespace clv;
using namespace clv::gfx;

namespace blb::rnd{
	/*static VertexBufferData transferVertexBufferData(VertexBufferData& source, const VertexLayout& newLayout){
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

                    case VertexElementType::jointIds:
                        vertexArray[i].getAttribute<VertexElementType::jointIds>() = source[i].getAttribute<VertexElementType::jointIds>();
                        break;

                    case VertexElementType::weights:
                        vertexArray[i].getAttribute<VertexElementType::weights>() = source[i].getAttribute<VertexElementType::weights>();
                        break;
                    default:
                        GARLIC_ASSERT(false, "{0}: Unknown vertex element", GARLIC_FUNCTION_NAME);
                        break;
				}
			}
		}

		return vertexArray;
	}*/

	/*static std::shared_ptr<clv::gfx::Buffer> createVertexBuffer(const VertexBufferData& vertexArray, GraphicsFactory& graphicsFactory) {
		BufferDescriptor vbdesc{};
		vbdesc.elementSize	= vertexArray.getLayout().size();
		vbdesc.bufferSize	= vertexArray.sizeBytes();
		vbdesc.bufferType	= BufferType::VertexBuffer;
		vbdesc.bufferUsage	= BufferUsage::Default;
		auto vertexBuffer	= graphicsFactory.createBuffer(vbdesc, vertexArray.data());

		return vertexBuffer;
	}*/

	/*static std::shared_ptr<clv::gfx::Buffer> createIndexBuffer(const std::vector<uint32_t>& indices, GraphicsFactory& graphicsFactory) {
		const std::size_t indexSize = sizeof(uint32_t);

		BufferDescriptor ibdesc{};
		ibdesc.elementSize	= indexSize;
		ibdesc.bufferSize	= indices.size() * indexSize;
		ibdesc.bufferType	= BufferType::IndexBuffer;
		ibdesc.bufferUsage	= BufferUsage::Default;
		auto indexBuffer	= graphicsFactory.createBuffer(ibdesc, indices.data());

		return indexBuffer;
	}*/

	Mesh::Mesh(const VertexBufferData& vbData, const std::vector<uint32_t>& indices, MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance))
		//, loadedBufferData(vbData)
		, indices(indices) {
	}

	Mesh::Mesh(const Mesh& other) = default;

	Mesh::Mesh(Mesh&& other) noexcept
		: materialInstance(std::move(other.materialInstance)){
		//, loadedBufferData(std::move(other.loadedBufferData)) {
		//vertexBufferMap	= std::move(other.vertexBufferMap);
		//indexBuffer		= std::move(other.indexBuffer);
		//indices			= std::move(other.indices);
	}

	Mesh& Mesh::operator=(const Mesh& other) = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		materialInstance	= std::move(other.materialInstance);
		//loadedBufferData	= std::move(other.loadedBufferData);
		//vertexBufferMap		= std::move(other.vertexBufferMap);
		indexBuffer			= std::move(other.indexBuffer);
		indices				= std::move(other.indices);

		return *this;
	}

	Mesh::~Mesh() = default;

	void Mesh::setMaterialInstance(MaterialInstance materialInstance) {
		this->materialInstance = std::move(materialInstance);
	}

	MaterialInstance& Mesh::getMaterialInstance() {
		return materialInstance;
	}

	uint32_t Mesh::getIndexCount(){
		return static_cast<uint32_t>(indices.size());
	}

	void Mesh::draw(CommandBuffer& commandBuffer, const VertexLayout& layout){
		/*if(indexBuffer == nullptr) {
			indexBuffer = createIndexBuffer(indices, *commandBuffer.getFactory());
		}

		if(vertexBufferMap.find(layout) == vertexBufferMap.end()) {
			vertexBufferMap[layout] = createVertexBuffer(transferVertexBufferData(loadedBufferData, layout), *commandBuffer.getFactory());
		}
		
		commandBuffer.bindVertexBuffer(*vertexBufferMap[layout], layout.size());
		commandBuffer.bindIndexBuffer(*indexBuffer);

		materialInstance.bind(commandBuffer);

		commandBuffer.drawIndexed(getIndexCount());*/
	}
}