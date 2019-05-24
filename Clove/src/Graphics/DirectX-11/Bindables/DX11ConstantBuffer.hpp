#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

#include <wrl.h>

struct ID3D11Buffer;

namespace clv::gfx{
	class Renderer;

	template <typename T>
	class DX11ConstantBuffer : public Bindable{
		//VARIABLES
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

		//FUNCTIONS
	public:
		DX11ConstantBuffer();
		DX11ConstantBuffer(const DX11ConstantBuffer& other) = delete;
		DX11ConstantBuffer(DX11ConstantBuffer&& other) noexcept;
		DX11ConstantBuffer<T>& operator=(const DX11ConstantBuffer& other) = delete;
		DX11ConstantBuffer<T>& operator=(DX11ConstantBuffer&& other) noexcept;
		virtual ~DX11ConstantBuffer();

		DX11ConstantBuffer(const T& data);

		void update(const T& data, Renderer& renderer);
	};

	template <typename T>
	class DX11VertexConstantBuffer : public DX11ConstantBuffer<T>{
		//DECLARATIONS
	public:
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		//FUNCTIONS
	public:
		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;
	};

	template <typename T>
	class DX11PixelConstantBuffer : public DX11ConstantBuffer<T>{
		//DECLARATIONS
	public:
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		//FUNCTIONS
	public:
		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;
	};
}

#include "DX11ConstantBuffer.inl"