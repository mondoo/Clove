#include "Clove/Graphics/Direct3D/D3DRenderFactory.hpp"

#include "Clove/Graphics/Direct3D/Resources/D3DBuffer.hpp"
#include "Clove/Graphics/Direct3D/Resources/D3DTexture.hpp"
#include "Clove/Graphics/Direct3D/D3DCommandBuffer.hpp"
#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DPipelineObject.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderTarget.hpp"
#include "Clove/Graphics/Direct3D/D3DShader.hpp"
#include "Clove/Graphics/Direct3D/D3DSurface.hpp"
#if CLV_DEBUG
	#include "Clove/Graphics/Direct3D/D3DRenderDevice.hpp"
#endif

#include <d3d11.h>

namespace clv::gfx::d3d{
	D3DRenderFactory::D3DRenderFactory(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3d11DeviceContext> d3dDeviceContext)
		: d3dDevice(d3dDevice), d3dDeviceContext(d3dDeviceContext){
	}

	D3DRenderFactory::~D3DRenderFactory() = default;

	std::shared_ptr<CommandBuffer> D3DRenderFactory::createCommandBuffer(const std::shared_ptr<RenderTarget>& renderTarget){
		return std::make_shared<D3DCommandBuffer>(d3dDeviceContext, renderTarget);
	}

	std::shared_ptr<CommandBuffer> D3DRenderFactory::createCommandBuffer(const Surface& surface){
		return std::make_shared<D3DCommandBuffer>(d3dDeviceContext, surface);
	}

	std::shared_ptr<Buffer> D3DRenderFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<D3DBuffer>(*d3dDevice.Get(), descriptor, data);
	}

	std::shared_ptr<Texture> D3DRenderFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<D3DTexture>(*d3dDevice.Get(), descriptor, pathToTexture);
	}

	std::shared_ptr<Texture> D3DRenderFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP){
		return std::make_shared<D3DTexture>(*d3dDevice.Get(), descriptor, data, BPP);
	}

	std::shared_ptr<PipelineObject> D3DRenderFactory::createPipelineObject(const std::shared_ptr<Shader>& shader){
		return std::make_shared<D3DPipelineObject>(*d3dDevice.Get(), shader);
	}

	std::shared_ptr<RenderTarget> D3DRenderFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_shared<D3DRenderTarget>(*d3dDevice.Get(), colourTexture, depthStencilTexture);
	}

	std::shared_ptr<Shader> D3DRenderFactory::createShader(const ShaderDescriptor& descriptor){
		return std::make_shared<D3DShader>(*d3dDevice.Get(), descriptor);
	}

	std::shared_ptr<Surface> D3DRenderFactory::createSurface(void* windowData){
		auto surface = std::make_unique<D3DSurface>(*d3dDevice.Get(), windowData);
		surface->onDeviceRemoved.bind(&D3DRenderFactory::onDeviceRemoved, this);
		return surface;
	}

	void D3DRenderFactory::onDeviceRemoved(){
		DX11_INFO_PROVIDER;
		throw DX11_DEVICE_REMOVED_EXCPTION(d3dDevice->GetDeviceRemovedReason());
	}
}