#include "clvpch.hpp"
#include "DX11Shader.hpp"

#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx{
	DX11Shader::DX11Shader(DX11Shader&& other) noexcept
		: vertCB(std::move(other.vertCB))
		, pixelCB(std::move(other.pixelCB)){
		shaders = std::move(other.shaders);
		vertexShader = other.vertexShader;
		other.vertexShader = nullptr;
	}

	DX11Shader& DX11Shader::operator=(DX11Shader&& other) noexcept{
		shaders = std::move(other.shaders);
		vertexShader = other.vertexShader;
		other.vertexShader = nullptr;

		vertCB = std::move(other.vertCB);
		pixelCB = std::move(other.pixelCB);

		return *this;
	}

	DX11Shader::~DX11Shader() = default;

	DX11Shader::DX11Shader(Renderer& renderer)
		: vertCB(renderer)
		, pixelCB(renderer){

		pixelColours = {
			{
				{1.0f, 0.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 0.3f, 1.0f, 1.0f},
				{1.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f, 1.0f},
			}
		};
	}

	void DX11Shader::bind(Renderer& renderer){
		for(const auto&[key, shader] : shaders){
			shader->bind(renderer);
		}

		vertCB.update(vertTransforms, renderer);
		vertCB.bind(renderer);

		pixelCB.update(pixelColours, renderer);
		pixelCB.bind(renderer);
	}

	void DX11Shader::unbind(){
	}

	void DX11Shader::attachShader(ShaderTypes type, Renderer& renderer){
		switch(type){
			case ShaderTypes::Vertex:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"Default-vs.cso", renderer);
					vertexShader = vs.get();
					shaders[type] = std::move(vs);
				}
				break;
			case ShaderTypes::Pixel:
				shaders[type] = std::make_unique<DX11PixelShader>(L"Default-ps.cso", renderer);
				break;

			default:
				CLV_ASSERT(false, "Unknown type! " __FUNCTION__);
				break;
		}
	}

	void DX11Shader::setWorldMatrix(const math::Matrix4f& world){
		vertTransforms.world = world;
	}

	void DX11Shader::setViewMatrix(const math::Matrix4f& view){
		vertTransforms.view = view;
	}

	void DX11Shader::setProjectionMatrix(const math::Matrix4f& projection){
		vertTransforms.projection = projection;
	}

	DX11VertexShader::DX11VertexShader(DX11VertexShader&& other) noexcept = default;

	DX11VertexShader& DX11VertexShader::operator=(DX11VertexShader&& other) noexcept = default;

	DX11VertexShader::~DX11VertexShader() = default;

	DX11VertexShader::DX11VertexShader(const std::wstring& path, Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			DX11_INFO_PROVIDER(dxrenderer);

			DX11_THROW_INFO(D3DReadFileToBlob(path.c_str(), &byteCode));
			DX11_THROW_INFO(dxrenderer->getDevice().CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &vertexShader));
		}
	}

	void DX11VertexShader::bind(Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			dxrenderer->getContext().VSSetShader(vertexShader.Get(), nullptr, 0u);
		}
	}

	void DX11VertexShader::unbind(){
	}

	DX11PixelShader::DX11PixelShader(DX11PixelShader&& other) noexcept = default;

	DX11PixelShader& DX11PixelShader::operator=(DX11PixelShader&& other) noexcept = default;

	DX11PixelShader::~DX11PixelShader() = default;

	DX11PixelShader::DX11PixelShader(const std::wstring& path, Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			DX11_INFO_PROVIDER(dxrenderer);

			DX11_THROW_INFO(D3DReadFileToBlob(path.c_str(), &byteCode));
			DX11_THROW_INFO(dxrenderer->getDevice().CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &pixelShader));
		}
	}

	void DX11PixelShader::bind(Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			dxrenderer->getContext().PSSetShader(pixelShader.Get(), nullptr, 0u);
		}
	}

	void DX11PixelShader::unbind(){
	}
}