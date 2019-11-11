#include "Core/Graphics/RenderCommand.hpp"

namespace clv::gfx{
	template<typename DataType>
	void Material::setData(BufferBindingPoint bindingPoint, DataType&& data, ShaderType shaderType){
		if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
			RenderCommand::updateBufferData(*iter->second.buffer, &data);
		} else{
			BufferDescriptor srdesc{};
			srdesc.elementSize = 0;
			srdesc.bufferSize = sizeof(data);
			BufferType bufferType = BufferType::ShaderResourceBuffer;
			BufferUsage bufferUsage = BufferUsage::Dynamic;
			auto buffer = RenderCommand::createBuffer(srdesc, &data);
			shaderData[bindingPoint] = { buffer, shaderType };
		}
	}
}