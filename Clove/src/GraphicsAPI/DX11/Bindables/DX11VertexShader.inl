namespace clv{
	namespace gfx{
		inline ID3DBlob* DX11VertexShader::getByteCode() const{
			return byteCode.Get();
		}
	}
}