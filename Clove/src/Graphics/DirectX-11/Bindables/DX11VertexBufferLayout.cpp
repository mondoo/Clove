#include "clvpch.hpp"
#include "DX11VertexBufferLayout.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11VertexBufferLayout::DX11VertexBufferLayout() = default;

	DX11VertexBufferLayout::DX11VertexBufferLayout(DX11VertexBufferLayout&& other) noexcept = default;

	DX11VertexBufferLayout& DX11VertexBufferLayout::operator=(DX11VertexBufferLayout&& other) noexcept = default;

	DX11VertexBufferLayout::~DX11VertexBufferLayout() = default;

	/*void DX11VertexBufferLayout::pushElement(const std::string& elementName, BufferElementFormat elementFormat){
		const UINT alignmentOffset = (elements.size() > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

		nameCache.push_back(elementName);
		if(nameCache.size() > 1){
			for(int i = 0; i < elements.size(); ++i){
				elements[i].SemanticName = nameCache[i].c_str();
			}
		}

		DXGI_FORMAT format;

		switch(elementFormat){
			case BufferElementFormat::FLOAT_2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;

			case BufferElementFormat::FLOAT_3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;

			default:
				CLV_ASSERT(false, "Unsupported type in {0}", __FUNCTION__);
				break;
				
		}
		
		elements.push_back({ nameCache.back().c_str(), 0, format, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}

	void DX11VertexBufferLayout::createLayout(Bindable& bindable){
		if(DX11Shader* shader = dynamic_cast<DX11Shader*>(&bindable)){
			Renderer& renderer = Application::get().getRenderer();
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
	}*/
}