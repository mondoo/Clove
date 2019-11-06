namespace clv::gfx{
	template<typename DataType>
	void MaterialInstance::setData(BufferBindingPoint bindingPoint, DataType&& data, gfx::ShaderType shaderType){
		if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
			if(auto sbo = std::dynamic_pointer_cast<ShaderResource<DataType>>(iter->second)){
				sbo->update(std::forward<DataType>(data));
				return;
			}
		}
		shaderData[bindingPoint] = BindableFactory::createShaderBufferObject<DataType>(shaderType, bindingPoint, std::forward<DataType>(data));
	}
}