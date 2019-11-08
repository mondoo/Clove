#include "D3DRenderFactory.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d::_11{
	D3DRenderFactory::D3DRenderFactory(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice)
		: d3dDevice(d3dDevice){
	}

	D3DRenderFactory::~D3DRenderFactory() = default;

	std::shared_ptr<IndexBuffer> D3DRenderFactory::createIndexBuffer(const IndexBufferDescriptor& descriptor, void* indices){
		return std::make_shared<IndexBuffer>(*this, descriptor, indices);
	}

	std::shared_ptr<VertexBuffer> D3DRenderFactory::createVertexBuffer(const VertexBufferDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<ShaderResource> D3DRenderFactory::createShaderResource(const ShaderResourceDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<Texture> D3DRenderFactory::createTexture(const TextureDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<Shader> D3DRenderFactory::createShader(const ShaderDescriptor& descriptor){
		//TODO
	}

	std::shared_ptr<Surface> D3DRenderFactory::createSurface(){
		//TODO + listen to device removed delegate
	}
	
}