#pragma once

#include "Core/Graphics/Bindables/ShaderBufferObject.hpp"

#include <wrl.h>

struct ID3D11Buffer;

namespace clv::gfx{
	class Renderer;
	
	template<typename T>
	class DX11ConstantBuffer : public ShaderBufferObject<T>{
		//VARIABLES
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

		uint32 bindingPoint = 0;

		//FUNCTIONS
	public:
		DX11ConstantBuffer() = delete;
		DX11ConstantBuffer(const DX11ConstantBuffer& other) = delete;
		DX11ConstantBuffer(DX11ConstantBuffer&& other) noexcept;
		DX11ConstantBuffer& operator=(const DX11ConstantBuffer& other) = delete;
		DX11ConstantBuffer& operator=(DX11ConstantBuffer&& other) noexcept;
		virtual ~DX11ConstantBuffer();

		DX11ConstantBuffer(uint32 bindingPoint);
		DX11ConstantBuffer(uint32 bindingPoint, T&& data);

		virtual void update(T&& data) override;
	};

	template<typename T>
	class DX11VertexConstantBuffer : public DX11ConstantBuffer<T>{
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		//FUNCTIONS
	public:
		virtual void bind() override;
	};

	template<typename T>
	class DX11PixelConstantBuffer : public DX11ConstantBuffer<T>{
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		//FUNCTIONS
	public:
		virtual void bind() override;
	};

	template<typename T>
	class DX11GeometryConstantBuffer : public DX11ConstantBuffer<T>{
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		//FUNCTIONS
	public:
		virtual void bind() override;
	};
}

#include "DX11ConstantBuffer.inl"