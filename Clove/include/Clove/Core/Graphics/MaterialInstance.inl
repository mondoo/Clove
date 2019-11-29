#include "Clove/Core/Graphics/RenderCommand.hpp"

namespace clv::gfx{
	template<typename DataType>
	void MaterialInstance::setData(BufferBindingPoint bindingPoint, DataType&& data, gfx::ShaderType shaderType){
		if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
			RenderCommand::updateBufferData(*iter->second.buffer, &data);
		} else{
			BufferDescriptor srdesc{};
			srdesc.elementSize	= 0;
			srdesc.bufferSize	= sizeof(data);
			srdesc.bufferType	= BufferType::ShaderResourceBuffer;
			srdesc.bufferUsage	= BufferUsage::Dynamic;

			auto buffer = RenderCommand::createBuffer(srdesc, &data);
			shaderData[bindingPoint] = { buffer, shaderType };
		}
	}
}