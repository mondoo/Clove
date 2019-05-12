#include "clvpch.hpp"
#include "DX11VertexShader.hpp"

#include "GraphicsAPI/DX11/DX11Exception.hpp"
#include "GraphicsAPI/DX11/DX11Renderer.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv{
	namespace gfx{
		DX11VertexShader::DX11VertexShader(DX11VertexShader&& other) noexcept = default;

		DX11VertexShader& DX11VertexShader::operator=(DX11VertexShader&& other) noexcept = default;

		DX11VertexShader::~DX11VertexShader() = default;

		DX11VertexShader::DX11VertexShader(const std::wstring& path, Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				DX11_INFO_PROVIDER(dxrenderer);

				DX11_THROW_INFO(D3DReadFileToBlob(path.c_str(), &byteCode));
				DX11_THROW_INFO(dxrenderer->getDevice().CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &vertexShader));
			}
		}

		void DX11VertexShader::bind(Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				dxrenderer->getContext().VSSetShader(vertexShader.Get(), nullptr, 0u);
			}
		}

		void DX11VertexShader::unbind(){
		}
	}
}