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
	D3DCommandBuffer::D3DCommandBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, const std::shared_ptr<RenderTarget>& renderTarget)
		: d3dContext(d3dContext){
		d3dRenderTarget = std::static_pointer_cast<D3DRenderTarget>(renderTarget);
	}

	D3DCommandBuffer::D3DCommandBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Surface& surface)
		: d3dContext(d3dContext){
		D3DSurface& d3dSurface = static_cast<D3DSurface&>(surface);

		d3dSurface.onBufferResizeRequested.bind(&D3DCommandBuffer::onSurfaceBufferResizeReuqested, this);
		finishBufferResize.bind(&D3DSurface::finishResizingBuffers, &d3dSurface);

		d3dRenderTarget = std::static_pointer_cast<D3DRenderTarget>(d3dSurface.getRenderTarget());
	}

	D3DCommandBuffer::D3DCommandBuffer(D3DCommandBuffer&& other) noexcept = default;

	D3DCommandBuffer& D3DCommandBuffer::operator=(D3DCommandBuffer&& other) noexcept = default;

	D3DCommandBuffer::~D3DCommandBuffer() = default;

	void D3DCommandBuffer::beginEncoding(){
		if(surfaceNeedsToResizeBuffers){
			d3dRenderTarget.reset();
			d3dRenderTarget = finishBufferResize.broadcast();
			surfaceNeedsToResizeBuffers = false;
		}

		d3dRenderTarget->lock();

		const auto beginCommand = [CAPTURE_CONTEXT, d3dRenderTarget = d3dRenderTarget.get()](){
			d3dRenderTarget->clear(*d3dContext);

			auto renderTargetView = d3dRenderTarget->getRenderTargetView();
			auto depthStencilView = d3dRenderTarget->getDepthStencilView();

			ID3D11RenderTargetView* rtViewArray[] = { renderTargetView.Get() };
			d3dContext->OMSetRenderTargets(1u, rtViewArray, depthStencilView.Get());
		};

		commands.push_back(beginCommand);
	}

	void D3DCommandBuffer::bindIndexBuffer(const Buffer& buffer){
		const auto bindIBCommand = [CAPTURE_CONTEXT, &buffer](){
			const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
			d3dContext->IASetIndexBuffer(d3dBuffer.getD3DBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
		};

		commands.push_back(bindIBCommand);
	}

	void D3DCommandBuffer::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		const auto bindVBCommand = [CAPTURE_CONTEXT, &buffer, stride](){
			const D3DBuffer& d3dBuffer = static_cast<const D3DBuffer&>(buffer);
			const auto& descriptor = d3dBuffer.getDescriptor();
			const UINT offset = 0u;
			d3dContext->IASetVertexBuffers(0u, 1u, d3dBuffer.getD3DBuffer().GetAddressOf(), &stride, &offset);
		};

		commands.push_back(bindVBCommand);
	}

	void D3DCommandBuffer::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		const auto bindSRBCommand = [CAPTURE_CONTEXT, &buffer, shaderType, bindingPoint](){
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
		};

		commands.push_back(bindSRBCommand);
	}

	void D3DCommandBuffer::bindPipelineObject(const PipelineObject& pipelineObject){
		const auto bindPOCommand = [CAPTURE_CONTEXT, &pipelineObject](){
			DX11_INFO_PROVIDER;

			const D3DPipelineObject& d3dPipelineObject = static_cast<const D3DPipelineObject&>(pipelineObject);

			Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice = nullptr;
			d3dContext->GetDevice(&d3dDevice);

			d3dContext->IASetInputLayout(d3dPipelineObject.getD3DInputLayout().Get());

			auto d3dShader = std::static_pointer_cast<D3DShader>(d3dPipelineObject.getShader());
			d3dContext->VSSetShader(d3dShader->getD3DVertexShader().Get(), nullptr, 0u);
			d3dContext->GSSetShader(d3dShader->getD3DGeometryShader().Get(), nullptr, 0u);
			d3dContext->PSSetShader(d3dShader->getD3DPixelShader().Get(), nullptr, 0u);

			Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
			DX11_THROW_INFO(d3dDevice->CreateBlendState(&d3dPipelineObject.getD3DBlendDesc(), &blendState));
			d3dContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

			Microsoft::WRL::ComPtr<ID3D11RasterizerState> rstate;
			DX11_THROW_INFO(d3dDevice->CreateRasterizerState(&d3dPipelineObject.getD3DRasterDesc(), &rstate));
			d3dContext->RSSetState(rstate.Get());

			d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		};

		commands.push_back(bindPOCommand);
	}

	void D3DCommandBuffer::bindTexture(const Texture* texture, const uint32 bindingPoint){
		const auto bindTextureCommand = [CAPTURE_CONTEXT, texture, bindingPoint](){
			if(const D3DTexture* d3dTexture = static_cast<const D3DTexture*>(texture)){
				d3dContext->PSSetShaderResources(bindingPoint, 1u, d3dTexture->getD3DShaderResourceView().GetAddressOf());
				d3dContext->PSSetSamplers(bindingPoint, 1u, d3dTexture->getD3DSampler().GetAddressOf());
			} else{
				ID3D11ShaderResourceView* resourceView[1] = { nullptr };
				ID3D11SamplerState* samplerState[1] = { nullptr };
				d3dContext->PSSetShaderResources(bindingPoint, 1u, resourceView);
				d3dContext->PSSetSamplers(bindingPoint, 1u, samplerState);
			}
		};

		commands.push_back(bindTextureCommand);
	}

	void D3DCommandBuffer::setViewport(const Viewport& viewport){
		const auto setVPCommand = [CAPTURE_CONTEXT, &viewport](){
			D3D11_VIEWPORT vp{};
			vp.TopLeftX = static_cast<FLOAT>(viewport.x);
			vp.TopLeftY = static_cast<FLOAT>(viewport.y);
			vp.Width	= static_cast<FLOAT>(viewport.width);
			vp.Height	= static_cast<FLOAT>(viewport.height);
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;

			d3dContext->RSSetViewports(1u, &vp);
		};

		commands.push_back(setVPCommand);
	}

	void D3DCommandBuffer::setDepthEnabled(bool enabled){
		const auto setDECommand = [CAPTURE_CONTEXT, enabled](){
			DX11_INFO_PROVIDER;

			Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice = nullptr;
			d3dContext->GetDevice(&d3dDevice);

			D3D11_DEPTH_STENCIL_DESC depthDesc{};
			depthDesc.DepthEnable		= enabled ? TRUE : FALSE;
			depthDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
			depthDesc.DepthFunc			= D3D11_COMPARISON_LESS;

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
			DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));
			d3dContext->OMSetDepthStencilState(dsstate.Get(), 1u);
		};

		commands.push_back(setDECommand);
	}

	void D3DCommandBuffer::drawIndexed(const uint32 count){
		const auto drawCommand = [CAPTURE_CONTEXT, count](){
			DX11_INFO_PROVIDER;
			DX11_THROW_INFO_ONLY(d3dContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
		};

		commands.push_back(drawCommand);
	}

	void D3DCommandBuffer::flushCommands(){
		for(auto& command : commands){
			command();
		}

		commands.clear();
		d3dRenderTarget->unlock();
	}

	void D3DCommandBuffer::onSurfaceBufferResizeReuqested(){
		surfaceNeedsToResizeBuffers = true;
	}
}