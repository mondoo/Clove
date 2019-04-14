namespace clv{
	template<typename T>
	void Renderer::setGlobalShaderUniform(const std::string& name, const T& value){
		if(currentShaderType != ShaderType::standard){
			prepareShader(ShaderType::standard);
		}

		currentShader->setUniform(name, value);
	}
}