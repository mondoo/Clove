namespace clv{
	template<typename T>
	void Material::setUniform(const std::string& name, const T& value){
		uniformMap[name] = value;
	}

	inline const gfx::ShaderType Material::getShaderType() const{
		return shaderType;
	}
}