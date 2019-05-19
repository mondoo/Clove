#include "clvpch.hpp"
#include "DX11VertexBufferLayout.hpp"

#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11VertexBufferLayout::DX11VertexBufferLayout() = default;

	DX11VertexBufferLayout::DX11VertexBufferLayout(DX11VertexBufferLayout&& other) noexcept = default;

	DX11VertexBufferLayout& DX11VertexBufferLayout::operator=(DX11VertexBufferLayout&& other) noexcept = default;

	DX11VertexBufferLayout::~DX11VertexBufferLayout() = default;

	void DX11VertexBufferLayout::pushElement(const std::string& elementName, BufferElementFormat elementFormat){
		const UINT alignmentOffset = (elements.size() > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

		nameCache.push_back(elementName);

		switch(elementFormat){
			case BufferElementFormat::FLOAT_2:
				elements.push_back({ nameCache.back().c_str(), 0, DXGI_FORMAT_R32G32_FLOAT, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
				break;

			case BufferElementFormat::FLOAT_3:
				elements.push_back({ nameCache.back().c_str(), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
				break;

			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				break;
		}
	}

	void DX11VertexBufferLayout::createLayout(Bindable& bindable, Renderer& renderer){
		if(DX11Shader* shader = dynamic_cast<DX11Shader*>(&bindable)){
			if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
				DX11_INFO_PROVIDER(dxrenderer);

				DX11_THROW_INFO(dxrenderer->getDevice().CreateInputLayout(
					elements.data(),
					static_cast<UINT>(elements.size()),
					shader->getVertexShader().getByteCode()->GetBufferPointer(),
					shader->getVertexShader().getByteCode()->GetBufferSize(),
					&inputLayout
				));
			}
		}
	}

	void DX11VertexBufferLayout::resetLayout(){
		elements.clear();
		inputLayout.Reset();
	}

	void DX11VertexBufferLayout::bind(Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			dxrenderer->getContext().IASetInputLayout(inputLayout.Get());
		}
	}

	void DX11VertexBufferLayout::unbind(){
	}
}