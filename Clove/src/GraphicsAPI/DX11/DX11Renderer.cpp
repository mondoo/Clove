#include "clvpch.hpp"
#include "DX11Renderer.hpp"

#include "Clove/Window.hpp"
#include "GraphicsAPI/DX11/DX11Exception.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace clv{
	namespace graphics{
		clv::graphics::DX11Renderer::DX11Renderer(const Window& window){
		#if CLV_PLATFORM_WINDOWS
			windowsHandle = reinterpret_cast<HWND>(window.getNativeWindow());

			DXGI_SWAP_CHAIN_DESC scd = { 0 };
			scd.BufferDesc.Width = 0;
			scd.BufferDesc.Height = 0;
			scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			scd.BufferDesc.RefreshRate.Numerator = 0;
			scd.BufferDesc.RefreshRate.Denominator = 0;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = 1;
			scd.OutputWindow = windowsHandle;
			scd.Windowed = TRUE;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = 0;

			HRESULT hr; //Used by DX11_THROW_FAILED macro

			UINT flags = 0;
		#if CLV_DEBUG
			flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

			DX11_THROW_INFO(D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				flags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&scd,
				&swapChain,
				&d3dDevice,
				nullptr,
				&d3dDeviceContext
			));

			Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
			DX11_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
			DX11_THROW_INFO(d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &target));
		#endif
		}

		DX11Renderer::~DX11Renderer() = default;

		void DX11Renderer::clear(){
			float colour[] = { 0.0f, 0.0f, 1.0f, 1.0f };
			d3dDeviceContext->ClearRenderTargetView(target.Get(), colour);
		}

		void DX11Renderer::drawScene(std::shared_ptr<scene::Scene> scene){
			HRESULT hr;

			drawTestTriangle();

		#if CLV_DEBUG
			infoManager.set();
		#endif
			if(FAILED(hr = swapChain->Present(1u, 0u))){
				if(hr == DXGI_ERROR_DEVICE_REMOVED){
					throw DX11_DEVICE_REMOVED_EXCPTION(d3dDevice->GetDeviceRemovedReason());
				} else{
					DX11_EXCEPT(hr);
				}
			}
		}

		void DX11Renderer::drawTestTriangle(){
			HRESULT hr;

			struct vertex{
				float x = 0;
				float y = 0;
			};

			const vertex vertices[] = {
				{ 0.0f,  0.5f},
				{ 0.5f, -0.5f},
				{-0.5f, -0.5f}
			};
			
			
			D3D11_BUFFER_DESC bd = { 0 };
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0u;
			bd.ByteWidth = sizeof(vertices);
			bd.StructureByteStride = sizeof(vertex);

			D3D11_SUBRESOURCE_DATA srd = { 0 };
			srd.pSysMem = vertices;

			Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
			
			DX11_THROW_INFO(d3dDevice->CreateBuffer(&bd, &srd, &vertexBuffer));

			//Bind VB into pipeline
			const UINT stride = sizeof(vertex);
			const UINT offset = 0u;
			d3dDeviceContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

			Microsoft::WRL::ComPtr<ID3DBlob> blob;

			//Create pixel shader
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
			DX11_THROW_INFO(D3DReadFileToBlob(L"Default-ps.cso", &blob));
			DX11_THROW_INFO(d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
			//Bind
			d3dDeviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

			//Create vertex shader
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
			DX11_THROW_INFO(D3DReadFileToBlob(L"Default-vs.cso", &blob));
			DX11_THROW_INFO(d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
			//Bind
			d3dDeviceContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

			//Input layout object (I'm guessing this is similar to what we do with opengl vertex array)
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
			const D3D11_INPUT_ELEMENT_DESC ied[] = {
				{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			DX11_THROW_INFO(d3dDevice->CreateInputLayout(
				ied, static_cast<UINT>(std::size(ied)),
				blob->GetBufferPointer(),
				blob->GetBufferSize(),
				&inputLayout
			));
			//Bind
			d3dDeviceContext->IASetInputLayout(inputLayout.Get());

			//Bind render target
			d3dDeviceContext->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);

			//Set primitive topology to triangle list (groups of 3 verticies)
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//Configure viewport (maps the render space to an area on screen)
			D3D11_VIEWPORT vp = { 0 };
			vp.Width = 1920; //will need to pull this from window or where to put the port
			vp.Height = 1080;
			vp.MinDepth = 0;
			vp.MaxDepth = 1;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			d3dDeviceContext->RSSetViewports(1u, &vp);

			DX11_THROW_INFO_ONLY(d3dDeviceContext->Draw(static_cast<UINT>(std::size(vertices)), 0u));
		}

		Renderer* Renderer::createDirectX11Renderer(const Window& window){
			return new DX11Renderer(window);
		}
	}
}