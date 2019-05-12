#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11VertexShader;

namespace clv{
	namespace gfx{
		class Renderer;

		class DX11VertexShader : public Bindable{
			//VARIABLES
		private:
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
			Microsoft::WRL::ComPtr<ID3DBlob> byteCode;

			//FUNCTIONS
		public:
			DX11VertexShader() = delete;
			DX11VertexShader(const DX11VertexShader& other) = delete;
			DX11VertexShader(DX11VertexShader&& other) noexcept;
			DX11VertexShader& operator=(const DX11VertexShader& other) = delete;
			DX11VertexShader& operator=(DX11VertexShader&& other) noexcept;
			~DX11VertexShader();

			DX11VertexShader(const std::wstring& path, Renderer* renderer);

			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;

			inline ID3DBlob* getByteCode() const;
		};
	}
}

#include "DX11VertexShader.inl"