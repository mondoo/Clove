#include "clvpch.hpp"
#include "DX11VertexBuffer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Graphics/DirectX-11/DX11Exception.hpp"
#include "Clove/Graphics/DirectX-11/DX11Renderer.hpp"
#include "Clove/Graphics/DirectX-11/Bindables/DX11Shader.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11VertexBuffer::DX11VertexBuffer(DX11VertexBuffer&& other) noexcept = default;

	DX11VertexBuffer& DX11VertexBuffer::operator=(DX11VertexBuffer&& other) noexcept = default;

	DX11VertexBuffer::~DX11VertexBuffer() = default;

	DX11VertexBuffer::DX11VertexBuffer(const VertexBufferData& bufferData, Shader& shader)
		: bufferData(bufferData){

		Renderer& renderer = Application::get().getRenderer();
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			if(DX11Shader* dxshader = dynamic_cast<DX11Shader*>(&shader)){
				DX11_INFO_PROVIDER(dxrenderer);

				D3D11_BUFFER_DESC vbd = {};
				vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vbd.Usage = D3D11_USAGE_DEFAULT;
				vbd.CPUAccessFlags = 0;
				vbd.MiscFlags = 0u;
				vbd.ByteWidth = this->bufferData.sizeBytes();
				vbd.StructureByteStride = this->bufferData.getLayout().size();

				D3D11_SUBRESOURCE_DATA vsrd = {};
				vsrd.pSysMem = this->bufferData.data();

				DX11_THROW_INFO(dxrenderer->getDevice().CreateBuffer(&vbd, &vsrd, &vertexBuffer));

				std::vector<D3D11_INPUT_ELEMENT_DESC> dxElements;
				dxElements.reserve(this->bufferData.getLayout().count());

				for(int i = 0; i < this->bufferData.getLayout().count(); ++i){
					VertexElement elem = this->bufferData.getLayout().resolve(i);
					
					const UINT alignmentOffset = (i > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

					/*DXGI_FORMAT format;
					std::string name;*/

					switch(elem.getType()){
						case VertexElementType::position2D:
							/*format = DXGI_FORMAT_R32G32_FLOAT;
							name = "Position";*/
							dxElements.push_back({ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });

							break;
						case VertexElementType::position3D:
							/*format = DXGI_FORMAT_R32G32B32_FLOAT;
							name = "Position";*/
							dxElements.push_back({ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });

							break;
						case VertexElementType::texture2D:
							/*format = DXGI_FORMAT_R32G32_FLOAT;
							name = "TexCoord";*/
							dxElements.push_back({ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });

							break;
						case VertexElementType::normal:
							/*format = DXGI_FORMAT_R32G32_FLOAT;
							name = "Normal";*/
							dxElements.push_back({ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });

							break;
						default:
							CLV_ASSERT(false, "Invalid element type");
							continue;
					}

					//dxElements.push_back({ name.c_str(), 0, format, 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
				}

				DX11_THROW_INFO(dxrenderer->getDevice().CreateInputLayout(
					dxElements.data(),
					static_cast<UINT>(dxElements.size()),
					dxshader->getVertexShader().getByteCode()->GetBufferPointer(),
					dxshader->getVertexShader().getByteCode()->GetBufferSize(),
					&inputLayout
				));
			}
		}
	}

	void DX11VertexBuffer::bind(Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			const UINT stride = bufferData.getLayout().size();
			const UINT offset = 0u;
			dxrenderer->getContext().IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
			dxrenderer->getContext().IASetInputLayout(inputLayout.Get());
		}
	}

	void DX11VertexBuffer::unbind(){
	}
}