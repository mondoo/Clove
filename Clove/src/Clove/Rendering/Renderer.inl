namespace clv{
	template<typename T>
	void Renderer::setGlobalShaderUniform(const std::string& name, const T& value, ShaderType shaderType){
		prepareShader(shaderType);
		currentShader->setUniform(name, value);
	}
}