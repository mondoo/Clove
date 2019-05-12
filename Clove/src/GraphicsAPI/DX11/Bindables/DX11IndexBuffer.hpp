#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include <wrl.h>

struct ID3D11Buffer;

namespace clv{
	namespace gfx{
		class Renderer;

		class DX11IndexBuffer : public Bindable{
			//VARIABLES
		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

			//FUNCTIONS
		public:
			DX11IndexBuffer() = delete;
			DX11IndexBuffer(const DX11IndexBuffer& other) = delete;
			DX11IndexBuffer(DX11IndexBuffer&& other) noexcept;
			DX11IndexBuffer& operator=(const DX11IndexBuffer& other) = delete;
			DX11IndexBuffer& operator=(DX11IndexBuffer&& other) noexcept;
			~DX11IndexBuffer();

			DX11IndexBuffer(const std::vector<unsigned short>& indices, Renderer* renderer);

			virtual void bind(Renderer* renderer) override;
			virtual void unbind() override;
		};
	}
}
