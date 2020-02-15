#include "Clove/Graphics/Direct3D/D3DGraphicsFactory.hpp"

#include "Clove/Graphics/Direct3D/Resources/D3DBuffer.hpp"
#include "Clove/Graphics/Direct3D/Resources/D3DTexture.hpp"
#include "Clove/Graphics/Direct3D/D3DCommandBuffer.hpp"
#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DPipelineObject.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderTarget.hpp"
#include "Clove/Graphics/Direct3D/D3DShader.hpp"
#include "Clove/Graphics/Direct3D/D3DSurface.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
	D3DGraphicsFactory::D3DGraphicsFactory(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dDeviceContext)
		: d3dDevice(d3dDevice), d3dDeviceContext(d3dDeviceContext){
	}

	D3DGraphicsFactory::~D3DGraphicsFactory() = default;

	std::shared_ptr<CommandBuffer> D3DGraphicsFactory::createCommandBuffer(){
		return std::make_shared<D3DCommandBuffer>(*d3dDevice.Get());
	}

	std::shared_ptr<Buffer> D3DGraphicsFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<D3DBuffer>(*d3dDevice.Get(), descriptor, data);
	}

	std::shared_ptr<Texture> D3DGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<D3DTexture>(*d3dDevice.Get(), descriptor, pathToTexture);
	}

	std::shared_ptr<Texture> D3DGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP){
		return std::make_shared<D3DTexture>(*d3dDevice.Get(), descriptor, data, BPP);
	}

	std::shared_ptr<PipelineObject> D3DGraphicsFactory::createPipelineObject(){
		return std::make_shared<D3DPipelineObject>(d3dDevice);
	}

	std::shared_ptr<RenderTarget> D3DGraphicsFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_shared<D3DRenderTarget>(*d3dDevice.Get(), colourTexture, depthStencilTexture);
	}

	std::shared_ptr<Shader> D3DGraphicsFactory::createShader(const ShaderDescriptor& descriptor, std::string_view pathToTexture){
		return std::make_shared<D3DShader>(*d3dDevice.Get(), descriptor, pathToTexture);
	}

	std::shared_ptr<Surface> D3DGraphicsFactory::createSurface(void* windowData){
		auto surface = std::make_unique<D3DSurface>(*d3dDevice.Get(), windowData);
		surface->onDeviceRemoved.bind(&D3DGraphicsFactory::onDeviceRemoved, this);
		return surface;
	}

	void D3DGraphicsFactory::onDeviceRemoved(){
		DX11_INFO_PROVIDER;
		throw DX11_DEVICE_REMOVED_EXCPTION(d3dDevice->GetDeviceRemovedReason());
	}
}