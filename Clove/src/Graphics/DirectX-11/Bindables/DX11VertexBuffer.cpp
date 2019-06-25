#include "clvpch.hpp"
#include "DX11VertexBuffer.hpp"

#include "Clove/Application.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#include "Graphics/DirectX-11/Bindables/DX11Shader.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DXGI_FORMAT getDXGIFormatFromType(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return DXGI_FORMAT_R32G32_FLOAT;
			case VertexElementType::position3D:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case VertexElementType::texture2D:
				return DXGI_FORMAT_R32G32_FLOAT;
			case VertexElementType::normal:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return DXGI_FORMAT_R32G32B32_FLOAT;
		}
	}

	DX11VertexBuffer::DX11VertexBuffer(DX11VertexBuffer&& other) noexcept = default;

	DX11VertexBuffer& DX11VertexBuffer::operator=(DX11VertexBuffer&& other) noexcept = default;

	DX11VertexBuffer::~DX11VertexBuffer() = default;

	DX11VertexBuffer::DX11VertexBuffer(const VertexBufferData& bufferData, Shader& shader)
		: VertexBuffer(bufferData){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&Application::get().getRenderer());
		DX11Shader* dxshader = static_cast<DX11Shader*>(&shader);

		DX11_INFO_PROVIDER(dxrenderer);

		D3D11_BUFFER_DESC vbd = {};
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0u;
		vbd.ByteWidth = bufferData.sizeBytes();
		vbd.StructureByteStride = bufferData.getLayout().size();

		D3D11_SUBRESOURCE_DATA vsrd = {};
		vsrd.pSysMem = bufferData.data();

		DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&vbd, &vsrd, &vertexBuffer));

		std::vector<D3D11_INPUT_ELEMENT_DESC> dxElements;
		dxElements.reserve(bufferData.getLayout().count());

		for(int i = 0; i < bufferData.getLayout().count(); ++i){
			const auto& element = bufferData.getLayout().resolve(i);
			const VertexElementType elementType = element.getType();
			const UINT alignmentOffset = (i > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

			dxElements.push_back({ element.getSemantic(), 0, getDXGIFormatFromType(elementType), 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		DX11_THROW_INFO(dxrenderer->getDevice().CreateInputLayout(
			dxElements.data(),
			static_cast<UINT>(dxElements.size()),
			dxshader->getVertexShader().getByteCode()->GetBufferPointer(),
			dxshader->getVertexShader().getByteCode()->GetBufferSize(),
			&inputLayout
		));
	}

	void DX11VertexBuffer::bind(Renderer& renderer){
		DX11Renderer* dxrenderer = static_cast<DX11Renderer*>(&renderer);
		const UINT stride = bufferData.getLayout().size();
		const UINT offset = 0u;
		dxrenderer->getContext().IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
		dxrenderer->getContext().IASetInputLayout(inputLayout.Get());
	}

	void DX11VertexBuffer::unbind(){
	}
}