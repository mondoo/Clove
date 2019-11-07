namespace clv::gfx{
	template<typename DataType>
	void Material::setData(BufferBindingPoint bindingPoint, DataType&& data, ShaderType shaderType){
		if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
			iter->second->setData(std::forward<DataType>(data));
		} else{
			ShaderResourceDescriptor srdesc = {};
			srdesc.bindingPoint = bindingPoint;
			srdesc.shaderType = shaderType;
			srdesc.bufferSize = sizeof(data);
			shaderData[bindingPoint] = RenderCommand::createShaderResource(srdesc);
			shaderData[bindingPoint]->setData(std::forward<DataType>(data));
		}
	}
}