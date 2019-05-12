#include "clvpch.hpp"
#include "DX11PixelShader.hpp"

#include "GraphicsAPI/DX11/DX11Exception.hpp"
#include "GraphicsAPI/DX11/DX11Renderer.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv{
	namespace gfx{
		DX11PixelShader::DX11PixelShader(DX11PixelShader&& other) noexcept = default;

		DX11PixelShader& DX11PixelShader::operator=(DX11PixelShader&& other) noexcept = default;

		DX11PixelShader::~DX11PixelShader() = default;

		DX11PixelShader::DX11PixelShader(const std::wstring& path, Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				DX11_INFO_PROVIDER(dxrenderer);

				DX11_THROW_INFO(D3DReadFileToBlob(path.c_str(), &byteCode));
				DX11_THROW_INFO(dxrenderer->getDevice().CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &pixelShader));
			}
		}

		void DX11PixelShader::bind(Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				dxrenderer->getContext().PSSetShader(pixelShader.Get(), nullptr, 0u);
			}
		}

		void DX11PixelShader::unbind(){
		}
	}
}