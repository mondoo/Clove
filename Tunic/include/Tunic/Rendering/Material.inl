#include "Clove/Graphics/Core/GraphicsFactory.hpp"

namespace tnc::rnd{
	template<typename DataType>
	void Material::setData(clv::gfx::BufferBindingPoint bindingPoint, DataType&& data, clv::gfx::ShaderStage shaderType){
		if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
			iter->second.buffer->updateData(&data);
		} else{
			clv::gfx::BufferDescriptor srdesc{};
			srdesc.elementSize	= 0;
			srdesc.bufferSize	= sizeof(data);
			srdesc.bufferType	= clv::gfx::BufferType::ShaderResourceBuffer;
			srdesc.bufferUsage	= clv::gfx::BufferUsage::Dynamic;

			auto buffer = graphicsFactory->createBuffer(srdesc, &data);
			shaderData[bindingPoint] = { buffer, shaderType };
		}
	}
}