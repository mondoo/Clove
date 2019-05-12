#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11PixelShader;

namespace clv{
	namespace gfx{
		class Renderer;

		class DX11PixelShader : public Bindable{
			//VARIABLES
		private:
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
			Microsoft::WRL::ComPtr<ID3DBlob> byteCode;

			//FUNCTIONS
		public:
			DX11PixelShader() = delete;
			DX11PixelShader(const DX11PixelShader& other) = delete;
			DX11PixelShader(DX11PixelShader&& other) noexcept;
			DX11PixelShader& operator=(const DX11PixelShader& other) = delete;
			DX11PixelShader& operator=(DX11PixelShader&& other) noexcept;
			~DX11PixelShader();

			DX11PixelShader(const std::wstring& path, Renderer* renderer);

			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;

			inline ID3DBlob* getByteCode() const;
		};
	}
}

#include "DX11PixelShader.inl"