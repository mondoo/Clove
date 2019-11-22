#include "D3DRenderDevice.hpp"

#include "Graphics/Direct3D/D3DException.hpp"
#include "Graphics/Direct3D/Resources/D3DBuffer.hpp"
#include "Graphics/Direct3D/Resources/D3DTexture.hpp"
#include "Graphics/Direct3D/D3DPipelineObject.hpp"
#include "Graphics/Direct3D/D3DRenderTarget.hpp"
#include "Graphics/Direct3D/D3DShader.hpp"
#include "Graphics/Direct3D/D3DSurface.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
#if CLV_DEBUG
	DXGIInfoManager D3DRenderDevice::infoManager;
#endif

	D3DRenderDevice::D3DRenderDevice(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
		: d3dContext(d3dContext){
	}

	D3DRenderDevice::~D3DRenderDevice() = default;

	void D3DRenderDevice::bindIndexBuffer(const Buffer& buffer){
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
		d3dContext->IASetIndexBuffer(d3dBuffer.getD3DBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	void D3DRenderDevice::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
		const auto& descriptor = d3dBuffer.getDescriptor();
		const UINT offset = 0u;
		d3dContext->IASetVertexBuffers(0u, 1u, d3dBuffer.getD3DBuffer().GetAddressOf(), &stride, &offset);
	}

	void D3DRenderDevice::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
		switch(shaderType){
			case ShaderType::Vertex:
				d3dContext->VSSetConstantBuffers(bindingPoint, 1u, d3dBuffer.getD3DBuffer().GetAddressOf());
				break;

			case ShaderType::Pixel:
				d3dContext->PSSetConstantBuffers(bindingPoint, 1u, d3dBuffer.getD3DBuffer().GetAddressOf());
				break;

			case ShaderType::Geometry:
				d3dContext->GSSetConstantBuffers(bindingPoint, 1u, d3dBuffer.getD3DBuffer().GetAddressOf());
				break;

			default:
				CLV_ASSERT(false, "Unhandled shader type in {0}", CLV_FUNCTION_NAME);
				break;
		}
	}

	void D3DRenderDevice::bindPipelineObject(const PipelineObject& pipelineObject){
		const D3DPipelineObject& d3dPipelineObject = static_cast<const D3DPipelineObject&>(pipelineObject);
		d3dContext->IASetInputLayout(d3dPipelineObject.getD3DInputLayout().Get());
		bindShader(*d3dPipelineObject.getShader());
	}

	void D3DRenderDevice::bindTexture(const Texture* texture, const uint32 bindingPoint){
		const D3DTexture* d3dTexture = static_cast<const D3DTexture*>(texture);

		if(d3dTexture){
			d3dContext->PSSetShaderResources(bindingPoint, 1u, d3dTexture->getD3DShaderResourceView().GetAddressOf());
			d3dContext->PSSetSamplers(bindingPoint, 1u, d3dTexture->getD3DSampler().GetAddressOf());
		} else{
			ID3D11ShaderResourceView* resourceView[1] = { nullptr };
			ID3D11SamplerState* samplerState[1] = { nullptr };
			d3dContext->PSSetShaderResources(bindingPoint, 1u, resourceView);
			d3dContext->PSSetSamplers(bindingPoint, 1u, samplerState);
		}
	}

	void D3DRenderDevice::bindShader(const Shader& shader){
		const D3DShader& d3dShader = static_cast<const D3DShader&>(shader);
		d3dContext->VSSetShader(d3dShader.getD3DVertexShader().Get(), nullptr, 0u);
		d3dContext->GSSetShader(d3dShader.getD3DGeometryShader().Get(), nullptr, 0u);
		d3dContext->PSSetShader(d3dShader.getD3DPixelShader().Get(), nullptr, 0u);
	}

	void D3DRenderDevice::updateBufferData(Buffer& buffer, const void* data){
		DX11_INFO_PROVIDER;
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);

		D3D11_MAPPED_SUBRESOURCE mappedSubResource{};
		DX11_THROW_INFO(d3dContext->Map(d3dBuffer.getD3DBuffer().Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
		
		memcpy(mappedSubResource.pData, data, d3dBuffer.getDescriptor().bufferSize);
		
		d3dContext->Unmap(d3dBuffer.getD3DBuffer().Get(), 0u);
	}

	void D3DRenderDevice::makeSurfaceCurrent(const std::shared_ptr<Surface>& surface){
		currentSurface = std::static_pointer_cast<D3DSurface>(surface);
	}

	void D3DRenderDevice::setRenderTarget(const RenderTarget* renderTarget){
		if(renderTarget){
			const D3DRenderTarget* d3dRenderTarget = static_cast<const D3DRenderTarget*>(renderTarget);
			auto renderTargetView = d3dRenderTarget->getRenderTargetView();
			auto depthStencilView = d3dRenderTarget->getDepthStencilView();
			
			ID3D11RenderTargetView* rtViewArray[] = { renderTargetView.Get() };
			d3dContext->OMSetRenderTargets(1u, rtViewArray, depthStencilView.Get());
		} else{
			ID3D11RenderTargetView* rtViewArray[] = { nullptr };
			d3dContext->OMSetRenderTargets(1u, rtViewArray, nullptr);
		}
	}

	void D3DRenderDevice::resetRenderTargetToDefault(){
		const D3DRenderTarget& d3dRenderTarget = currentSurface->getTarget();
		auto renderTargetView = d3dRenderTarget.getRenderTargetView();
		auto depthStencilView = d3dRenderTarget.getDepthStencilView();

		ID3D11RenderTargetView* rtViewArray[] = { renderTargetView.Get() };
		d3dContext->OMSetRenderTargets(1u, rtViewArray, depthStencilView.Get());
	}

	void D3DRenderDevice::setViewport(const Viewport& viewport){
		D3D11_VIEWPORT vp{};
		vp.TopLeftX = static_cast<FLOAT>(viewport.x);
		vp.TopLeftY = static_cast<FLOAT>(viewport.y);
		vp.Width	= static_cast<FLOAT>(viewport.width);
		vp.Height	= static_cast<FLOAT>(viewport.height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		d3dContext->RSSetViewports(1u, &vp);
	}

	void D3DRenderDevice::clear(){
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		d3dContext->OMGetRenderTargets(1u, &renderTargetView, &depthStencilView);

		d3dContext->ClearRenderTargetView(renderTargetView.Get(), mth::valuePtr(clearColour));
		d3dContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
	}

	void D3DRenderDevice::drawIndexed(const uint32 count){
		DX11_THROW_INFO_ONLY(d3dContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
	}

	void D3DRenderDevice::setClearColour(const mth::vec4f& colour){
		clearColour = colour;
	}

	void D3DRenderDevice::setDepthBuffer(bool enabled){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		UINT stencilRef;

		d3dContext->OMGetDepthStencilState(&depthStencilState, &stencilRef);

		ID3D11Device* d3dDevice;
		D3D11_DEPTH_STENCIL_DESC depthDesc{};

		depthStencilState->GetDevice(&d3dDevice);
		depthStencilState->GetDesc(&depthDesc);

		depthDesc.DepthEnable = enabled ? TRUE : FALSE;
		DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &depthStencilState));
		d3dContext->OMSetDepthStencilState(depthStencilState.Get(), stencilRef);
	}

	void D3DRenderDevice::setBlendState(bool enabled){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		FLOAT blendFactor[4];
		UINT sampleMask;

		d3dContext->OMGetBlendState(&blendState, blendFactor, &sampleMask);

		ID3D11Device* d3dDevice;
		D3D11_BLEND_DESC blendDesc{};

		blendState->GetDevice(&d3dDevice);
		blendState->GetDesc(&blendDesc);

		blendDesc.RenderTarget[0].BlendEnable = enabled ? TRUE : FALSE;
		DX11_THROW_INFO(d3dDevice->CreateBlendState(&blendDesc, &blendState));
		d3dContext->OMSetBlendState(blendState.Get(), blendFactor, sampleMask);
	}

#if CLV_DEBUG
	DXGIInfoManager& D3DRenderDevice::getInfoManager(){
		return infoManager;
	}
#endif
}