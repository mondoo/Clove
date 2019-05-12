#include "clvpch.hpp"
#include "DX11InputLayout.hpp"

#include "GraphicsAPI/DX11/DX11Exception.hpp"
#include "GraphicsAPI/DX11/DX11Renderer.hpp"

#include <d3d11.h>

namespace clv{
	namespace gfx{
		DX11InputLayout::DX11InputLayout(DX11InputLayout&& other) noexcept = default;

		DX11InputLayout& DX11InputLayout::operator=(DX11InputLayout&& other) noexcept = default;

		DX11InputLayout::~DX11InputLayout() = default;

		DX11InputLayout::DX11InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, ID3DBlob* vertexShaderByteCode, Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				DX11_INFO_PROVIDER(dxrenderer);

				DX11_THROW_INFO(dxrenderer->getDevice().CreateInputLayout(
					elements.data(),
					static_cast<UINT>(elements.size()),
					vertexShaderByteCode->GetBufferPointer(),
					vertexShaderByteCode->GetBufferSize(),
					&inputLayout
				));
			}
		}

		void DX11InputLayout::bind(Renderer* renderer){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(renderer)){
				dxrenderer->getContext().IASetInputLayout(inputLayout.Get());
			}
		}

		void DX11InputLayout::unbind(){
		}
	}
}