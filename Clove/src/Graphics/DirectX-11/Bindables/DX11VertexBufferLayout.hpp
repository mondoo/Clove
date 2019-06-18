#pragma once

//#include "Clove/Graphics/Bindables/VertexBufferLayout.hpp"

#include <wrl.h>


struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace clv::gfx{
	class Renderer;

	class DX11VertexBufferLayout/* : public VertexLayout*/{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;

		std::vector<std::string> nameCache;

		//FUNCTIONS
	public:
		DX11VertexBufferLayout();
		DX11VertexBufferLayout(const DX11VertexBufferLayout& other) = delete;
		DX11VertexBufferLayout(DX11VertexBufferLayout&& other) noexcept;
		DX11VertexBufferLayout& operator=(const DX11VertexBufferLayout& other) = delete;
		DX11VertexBufferLayout& operator=(DX11VertexBufferLayout&& other) noexcept;
		~DX11VertexBufferLayout();

		/*virtual void pushElement(const std::string& elementName, BufferElementFormat elementFormat) override;
		virtual void createLayout(Bindable& bindable) override;
		virtual void resetLayout() override;

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;*/
	};
}