namespace clv::gfx{
	inline ID3D11Device& DX11Renderer::getDevice() const{
		CLV_ASSERT(d3dDevice != nullptr, __FUNCTION__" called with null device");
		return *d3dDevice.Get();
	}

	inline ID3D11DeviceContext& DX11Renderer::getContext() const{
		CLV_ASSERT(d3dDevice != nullptr, __FUNCTION__" called with null context");
		return *d3dDeviceContext.Get();
	}

#if CLV_DEBUG
	inline DXGIInfoManager& DX11Renderer::getInfoManager(){
		return infoManager;
	}
#endif
}