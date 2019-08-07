#pragma once

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

#include <wrl.h>

struct ID3D11Buffer;

namespace clv::gfx{
	class Renderer;

	class DX11ConstantBuffer : public ShaderBufferObject{
		//VARIABLES
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

		unsigned int bindingPoint = 0;

		//FUNCTIONS
	public:
		DX11ConstantBuffer() = delete;
		DX11ConstantBuffer(const DX11ConstantBuffer& other) = delete;
		DX11ConstantBuffer(DX11ConstantBuffer&& other) noexcept;
		DX11ConstantBuffer& operator=(const DX11ConstantBuffer& other) = delete;
		DX11ConstantBuffer& operator=(DX11ConstantBuffer&& other) noexcept;
		virtual ~DX11ConstantBuffer();

		DX11ConstantBuffer(unsigned int bindingPoint);
		DX11ConstantBuffer(unsigned int bindingPoint, const MaterialData& data);

		virtual void update(const MaterialData& data) override;
	};

	class DX11VertexConstantBuffer : public DX11ConstantBuffer{
		//FUNCTIONS
	public:
		virtual void bind() override;
	};

	class DX11PixelConstantBuffer : public DX11ConstantBuffer{
		//FUNCTIONS
	public:
		virtual void bind() override;
	};
}