#include "clvpch.hpp"
#include "DX11Renderer.hpp"

#include "Clove/Window.hpp"
#include "GraphicsAPI/DX11/DX11Exception.hpp"

#include "Clove/Input/Input.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//temp
#include "GraphicsAPI/DX11/Bindables/DX11VertexBuffer.hpp"
#include "GraphicsAPI/DX11/Bindables/DX11IndexBuffer.hpp"
#include "GraphicsAPI/DX11/Bindables/DX11ConstantBuffer.hpp"
#include "GraphicsAPI/DX11/Bindables/DX11VertexShader.hpp"
#include "GraphicsAPI/DX11/Bindables/DX11PixelShader.hpp"
#include "GraphicsAPI/DX11/Bindables/DX11InputLayout.hpp"

namespace clv{
	namespace gfx{
		DX11Renderer::~DX11Renderer() = default;

		clv::gfx::DX11Renderer::DX11Renderer(const Window& window){
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

			//Get access to the texture subresource (back buffer)
			Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
			DX11_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
			DX11_THROW_INFO(d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &target));

			//Create depth stencil state
			D3D11_DEPTH_STENCIL_DESC depthDesc = {};
			depthDesc.DepthEnable = TRUE;
			depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
			DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));

			//Bind depth state
			d3dDeviceContext->OMSetDepthStencilState(dsstate.Get(), 1u);

			//Create depth stencil texture
			D3D11_TEXTURE2D_DESC depthTexDesc = {};
			depthTexDesc.Width = 1920u;
			depthTexDesc.Height = 1080u; //should match the swap chain (we used 0 to let d3d figure it out)
			depthTexDesc.MipLevels = 1u;
			depthTexDesc.ArraySize = 1u;
			depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT; //D for depth
			depthTexDesc.SampleDesc.Count = 1u;
			depthTexDesc.SampleDesc.Quality = 0u;
			depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
			depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
			DX11_THROW_INFO(d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &depthStencil));

			//Create view of depth stencil texture
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0u;

			DX11_THROW_INFO(d3dDevice->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, &dsv));

			//Bind depth stencil view to output merger
			d3dDeviceContext->OMSetRenderTargets(1u, target.GetAddressOf(), dsv.Get());

		#endif
		}

		void DX11Renderer::clear(){
			float colour[] = { 0.0f, 0.0f, 1.0f, 1.0f };
			d3dDeviceContext->ClearRenderTargetView(target.Get(), colour);
			d3dDeviceContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
		}

		void DX11Renderer::drawScene(const std::shared_ptr<scene::Scene>& scene){
			HRESULT hr;

			auto[fmx, fmy] = input::getMousePosition();
			//Scaling by viewport size
			fmx = (fmx / 960) - 1.0f;
			fmy = (fmy / 540) - 1.0f;

			static float angle = 0.0f;
			angle += 0.01f;
			drawTestTriangle(angle, 0, 0);

			//drawTestTriangle(angle, fmx, -fmy);

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

		void DX11Renderer::drawTestTriangle(float angle, float x, float z){
			HRESULT hr;

			/*struct vertex{
				struct{
					float x = 0;
					float y = 0;
					float z = 0;
				} pos;
			};*/

			//Create vertex buffer
			const std::vector<float> vertices = {
				-1.0f, -1.0f, -1.0f , //0
				 1.0f, -1.0f, -1.0f , //1
				-1.0f,  1.0f, -1.0f , //2
				 1.0f,  1.0f, -1.0f , //3
				-1.0f, -1.0f,  1.0f , //4
				 1.0f, -1.0f,  1.0f , //5
				-1.0f,  1.0f,  1.0f , //6
				 1.0f,  1.0f,  1.0f , //7
			};

			DX11VertexBuffer vb(vertices, this);

			//Bind VB into pipeline
			vb.bind(this);

			//Create index buffer
			const std::vector<unsigned short> indices = {
				0, 2, 1,	2, 3, 1,
				1, 3, 5,	3, 7, 5,
				2, 6, 3,	3, 6, 7,
				4, 5, 7,	4, 7, 6,
				0, 4, 2,	2, 4, 6,
				0, 1, 4,	1, 5, 4
			};

			DX11IndexBuffer ib(indices, this);

			//Bind IB into pipeline
			ib.bind(this);

			//Create constant buffer for transformation matrix (like with uniforms looks like dx calls them constants)
			struct ConstantBuffer{
				DirectX::XMMATRIX transform;
			};
			const ConstantBuffer cb = {
				{ //rot scale trans for dx
					DirectX::XMMatrixTranspose(
						DirectX::XMMatrixRotationZ(angle) * //Need to transpose matrix from row major to colum major for the gpu
						DirectX::XMMatrixRotationX(angle) *
						DirectX::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
						DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 10.0f)
					)
				}
			};
			VertexConstantBuffer vbc(cb, this);

			//Bind constant buffer to vertex shader
			vbc.bind(this);

			//Create constant buffer for face colour
			struct ConstantBuffer2{
				struct{
					float r = 0.0f;
					float g = 0.0f;
					float b = 0.0f;
					float a = 0.0f;
				} faceColours[6];
			};
			const ConstantBuffer2 cb2{
				{
					{1.0f, 0.0f, 1.0f, 1.0f},
					{1.0f, 0.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 0.0f, 1.0f},
					{0.0f, 0.3f, 1.0f, 1.0f},
					{1.0f, 1.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 1.0f, 1.0f},
				}
			};
			PixelConstantBuffer pcb(cb2, this);

			//Bind constant buffer to the pixel shader
			pcb.update(cb2, this);
			pcb.bind(this);

			//Create pixel shader
			DX11PixelShader ps(L"Default-ps.cso", this);
			//Bind
			ps.bind(this);

			//Create vertex shader
			DX11VertexShader vs(L"Default-vs.cso", this);
			
			//Bind
			vs.bind(this);

			//Input layout object (I'm guessing this is similar to what we do with opengl vertex array) (it is, think vertexattribpointer with the vb layout)
			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
				{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			DX11InputLayout ipl(ied, vs.getByteCode(), this);
			
			//Bind
			ipl.bind(this);

			//Set primitive topology to triangle list (groups of 3 verticies)
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//Configure viewport (maps the render space to an area on screen)
			D3D11_VIEWPORT vp = { 0 };
			vp.Width = 1920; //will need to pull this from window or where to put/size the view port
			vp.Height = 1080;
			vp.MinDepth = 0;
			vp.MaxDepth = 1;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			d3dDeviceContext->RSSetViewports(1u, &vp);

			DX11_THROW_INFO_ONLY(d3dDeviceContext->DrawIndexed(static_cast<UINT>(indices.size()), 0u, 0u));
		}
	}
}