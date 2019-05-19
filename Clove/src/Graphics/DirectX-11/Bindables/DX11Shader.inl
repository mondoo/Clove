namespace clv::gfx{
	inline DX11VertexShader& DX11Shader::getVertexShader(){
		CLV_ASSERT(vertexShader != nullptr, "No vertex shader attached!");
		return *vertexShader;
	}

	inline ID3DBlob* DX11VertexShader::getByteCode() const{
		return byteCode.Get();
	}

	inline ID3DBlob* DX11PixelShader::getByteCode() const{
		return byteCode.Get();
	}
}