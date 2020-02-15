#include "Clove/Graphics/Direct3D/D3DCommandBuffer.hpp"

#include "Clove/Graphics/Core/Surface.hpp"
#include "Clove/Graphics/Direct3D/Resources/D3DBuffer.hpp"
#include "Clove/Graphics/Direct3D/Resources/D3DTexture.hpp"
#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DPipelineObject.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderTarget.hpp"
#include "Clove/Graphics/Direct3D/D3DShader.hpp"
#include "Clove/Graphics/Direct3D/D3DSurface.hpp"

#include <d3d11.h>

#define CAPTURE_CONTEXT d3dContext = d3dContext.Get()

namespace clv::gfx::d3d{
	D3DCommandBuffer::D3DCommandBuffer(ID3D11Device& d3dDevice){
		DX11_INFO_PROVIDER;
		d3dDevice.GetImmediateContext(&immediateContext);
		DX11_THROW_INFO(d3dDevice.CreateDeferredContext(0u, &deferredContext));
	}

	D3DCommandBuffer::D3DCommandBuffer(D3DCommandBuffer&& other) noexcept = default;

	D3DCommandBuffer& D3DCommandBuffer::operator=(D3DCommandBuffer&& other) noexcept = default;

	D3DCommandBuffer::~D3DCommandBuffer() = default;

	void D3DCommandBuffer::beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget){
		d3dRenderTarget = std::static_pointer_cast<D3DRenderTarget>(renderTarget);

		auto renderTargetView = d3dRenderTarget->getRenderTargetView();
		auto depthStencilView = d3dRenderTarget->getDepthStencilView();

		ID3D11RenderTargetView* rtViewArray[] = { renderTargetView.Get() };
		deferredContext->OMSetRenderTargets(1u, rtViewArray, depthStencilView.Get());
	}

	void D3DCommandBuffer::clearTarget(){
		auto renderTargetView = d3dRenderTarget->getRenderTargetView();
		auto depthStencilView = d3dRenderTarget->getDepthStencilView();

		if (renderTargetView){
			deferredContext->ClearRenderTargetView(renderTargetView.Get(), mth::valuePtr(d3dRenderTarget->getClearColour()));
		}
		if (depthStencilView){
			deferredContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
		}
	}

	void D3DCommandBuffer::updateBufferData(const Buffer& buffer, const void* data){
		DX11_INFO_PROVIDER;

		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);

		D3D11_MAPPED_SUBRESOURCE mappedSubResource{};
		DX11_THROW_INFO(deferredContext->Map(d3dBuffer.getD3DBuffer().Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));

		memcpy(mappedSubResource.pData, data, d3dBuffer.getDescriptor().bufferSize);

		deferredContext->Unmap(d3dBuffer.getD3DBuffer().Get(), 0u);
	}

	void D3DCommandBuffer::bindIndexBuffer(const Buffer& buffer){
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
		deferredContext->IASetIndexBuffer(d3dBuffer.getD3DBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	void D3DCommandBuffer::bindVertexBuffer(const Buffer& buffer, const uint32_t stride){
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
		const auto& descriptor = d3dBuffer.getDescriptor();
		const UINT offset = 0u;
		deferredContext->IASetVertexBuffers(0u, 1u, d3dBuffer.getD3DBuffer().GetAddressOf(), &stride, &offset);
	}

	void D3DCommandBuffer::bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint){
		const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
		switch(shaderType){
			case ShaderStage::Vertex:
				deferredContext->VSSetConstantBuffers(bindingPoint, 1u, d3dBuffer.getD3DBuffer().GetAddressOf());
				break;

			case ShaderStage::Pixel:
				deferredContext->PSSetConstantBuffers(bindingPoint, 1u, d3dBuffer.getD3DBuffer().GetAddressOf());
				break;

			case ShaderStage::Geometry:
				deferredContext->GSSetConstantBuffers(bindingPoint, 1u, d3dBuffer.getD3DBuffer().GetAddressOf());
				break;

			default:
				CLV_ASSERT(false, "Unhandled shader type in {0}", CLV_FUNCTION_NAME);
				break;
		}
	}

	void D3DCommandBuffer::bindPipelineObject(const PipelineObject& pipelineObject){
		DX11_INFO_PROVIDER;

		const D3DPipelineObject& d3dPipelineObject = static_cast<const D3DPipelineObject&>(pipelineObject);

		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice = nullptr;
		deferredContext->GetDevice(&d3dDevice);

		deferredContext->IASetInputLayout(d3dPipelineObject.getD3DInputLayout().Get());

		deferredContext->VSSetShader(d3dPipelineObject.getD3DVertexShader().Get(), nullptr, 0u);
		deferredContext->GSSetShader(d3dPipelineObject.getD3DGeometryShader().Get(), nullptr, 0u);
		deferredContext->PSSetShader(d3dPipelineObject.getD3DPixelShader().Get(), nullptr, 0u);

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		DX11_THROW_INFO(d3dDevice->CreateBlendState(&d3dPipelineObject.getD3DBlendDesc(), &blendState));
		deferredContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rstate;
		DX11_THROW_INFO(d3dDevice->CreateRasterizerState(&d3dPipelineObject.getD3DRasterDesc(), &rstate));
		deferredContext->RSSetState(rstate.Get());

		deferredContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void D3DCommandBuffer::bindTexture(const Texture* texture, const uint32_t bindingPoint){
		if(const D3DTexture* d3dTexture = static_cast<const D3DTexture*>(texture)){
			deferredContext->PSSetShaderResources(bindingPoint, 1u, d3dTexture->getD3DShaderResourceView().GetAddressOf());
			deferredContext->PSSetSamplers(bindingPoint, 1u, d3dTexture->getD3DSampler().GetAddressOf());
		} else{
			ID3D11ShaderResourceView* resourceView[1] = { nullptr };
			ID3D11SamplerState* samplerState[1] = { nullptr };
			deferredContext->PSSetShaderResources(bindingPoint, 1u, resourceView);
			deferredContext->PSSetSamplers(bindingPoint, 1u, samplerState);
		}
	}

	void D3DCommandBuffer::setViewport(const Viewport& viewport){
		D3D11_VIEWPORT vp{};
		vp.TopLeftX = static_cast<FLOAT>(viewport.x);
		vp.TopLeftY = static_cast<FLOAT>(viewport.y);
		vp.Width	= static_cast<FLOAT>(viewport.width);
		vp.Height	= static_cast<FLOAT>(viewport.height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		deferredContext->RSSetViewports(1u, &vp);
	}

	void D3DCommandBuffer::setDepthEnabled(bool enabled){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice = nullptr;
		deferredContext->GetDevice(&d3dDevice);

		D3D11_DEPTH_STENCIL_DESC depthDesc{};
		depthDesc.DepthEnable = enabled ? TRUE : FALSE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
		DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));
		deferredContext->OMSetDepthStencilState(dsstate.Get(), 1u);
	}

	void D3DCommandBuffer::drawIndexed(const uint32_t count){
		DX11_INFO_PROVIDER;
		DX11_THROW_INFO_ONLY(deferredContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
	}

	void D3DCommandBuffer::endEncoding(){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11CommandList> commandList = nullptr;
		DX11_THROW_INFO(deferredContext->FinishCommandList(TRUE, &commandList));

		immediateContext->ExecuteCommandList(commandList.Get(), TRUE);

		d3dRenderTarget.reset();
	}
}