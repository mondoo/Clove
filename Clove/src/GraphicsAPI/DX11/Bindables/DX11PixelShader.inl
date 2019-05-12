namespace clv{
	namespace gfx{
		inline ID3DBlob* DX11PixelShader::getByteCode() const{
			return byteCode.Get();
		}
	}
}