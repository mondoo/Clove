#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace clv{
	namespace gfx{
		class Renderer;

		class DX11InputLayout : public Bindable{
			//VARIABLES
		private:
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

			//FUNCTIONS
		public:
			DX11InputLayout() = delete;
			DX11InputLayout(const DX11InputLayout& other) = delete;
			DX11InputLayout(DX11InputLayout&& other) noexcept;
			DX11InputLayout& operator=(const DX11InputLayout& other) = delete;
			DX11InputLayout& operator=(DX11InputLayout&& other) noexcept;
			~DX11InputLayout();

			DX11InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, ID3DBlob* vertexShaderByteCode, Renderer* renderer);
		
			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;
		};
	}
}