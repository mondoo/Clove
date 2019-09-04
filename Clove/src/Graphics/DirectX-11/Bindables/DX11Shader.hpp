#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace clv::gfx{
	class Renderer;

	class DX11ShaderElement;
	class DX11VertexShader;
	//class DX11PixelShader;

	class DX11Shader : public Shader{
		//VARIABLES
	private:
		std::unordered_map<ShaderType, std::unique_ptr<DX11ShaderElement>> shaders;
		DX11VertexShader* vertexShader = nullptr;
		//DX11PixelShader* pixelShader = nullptr;

		/*
		perhaps not cache this off. and just generate the shader reflection data as the shader is being constructed
		*/

		ShaderReflectionData reflectionData;

		//FUNCTIONS	
	public:
		DX11Shader() = delete;
		DX11Shader(const DX11Shader& other) = delete;
		DX11Shader(DX11Shader&& other) noexcept;
		DX11Shader& operator=(const DX11Shader& other) = delete;
		DX11Shader& operator=(DX11Shader&& other) noexcept;
		virtual ~DX11Shader();

		DX11Shader(ShaderStyle style);

		virtual void bind() override;
		
		virtual ShaderReflectionData getReflectionData() override;

		DX11VertexShader& getVertexShader();

	private:
		void initialise(ShaderStyle style);

		void reflectVertexShader(ID3DBlob* byteCode);
		void reflectPixelShader(ID3DBlob* byteCode);

		//BufferVariableType getVariableType(const D3D11_SHADER_TYPE_DESC& shaderTypeDesc);
	};

	//TODO: move all of this out of this header?

	class DX11ShaderElement : public Bindable{
		//FUNCTIONS	
	public:
		DX11ShaderElement();
		DX11ShaderElement(const DX11ShaderElement& other) = delete;
		DX11ShaderElement(DX11ShaderElement&& other) noexcept;
		DX11ShaderElement& operator=(const DX11ShaderElement& other) = delete;
		DX11ShaderElement& operator=(DX11ShaderElement&& other) noexcept;
		virtual ~DX11ShaderElement();

		virtual ID3DBlob* getByteCode() const = 0;
	};

	class DX11VertexShader : public DX11ShaderElement{
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

		DX11VertexShader(const std::wstring& path);

		virtual void bind() override;

		virtual ID3DBlob* getByteCode() const override;
	};

	class DX11PixelShader : public DX11ShaderElement{
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

		DX11PixelShader(const std::wstring& path);

		virtual void bind() override;

		virtual ID3DBlob* getByteCode() const override;
	};
}