#pragma once

#include "Core/Graphics/Shader.hpp"

#include <wrl.h>
#include <d3d.h>

namespace clv::gfx::d3d::_11{
	class DX11Shader : public Shader{
		//VARIABLES
	private:
		std::unordered_map<ShaderType, std::unique_ptr<D3DShaderElement>> d3dShaders;
		D3DVertexShader* vertexShader = nullptr;

		ShaderDescriptor descriptor;
		ShaderReflectionData reflectionData;

		//FUNCTIONS	
	public:
		DX11Shader() = delete;
		DX11Shader(ID3D11Device& d3dDevice, const ShaderDescriptor& descriptor);
		DX11Shader(const DX11Shader& other) = delete;
		DX11Shader(DX11Shader&& other) noexcept;
		DX11Shader& operator=(const DX11Shader& other) = delete;
		DX11Shader& operator=(DX11Shader&& other) noexcept;
		virtual ~DX11Shader();

		virtual const ShaderDescriptor& getDescriptor() const override;
		virtual ShaderReflectionData getReflectionData() const override;

		const std::unordered_map<ShaderType, std::unique_ptr<D3DShaderElement>>& getD3DShaders() const;

	private:
		void initialise(ID3D11Device& d3dDevice, ShaderStyle style);
	};

	//TODO: move all of this out of this header

	class D3DShaderElement{
		//FUNCTIONS	
	public:
		virtual const BYTE* getBytePointer() const = 0;
		virtual SIZE_T getByteSize() const = 0;
	};

	class D3DVertexShader : public D3DShaderElement{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		
		const BYTE* shaderByteData;
		SIZE_T shaderByteSize;

		//FUNCTIONS
	public:
		D3DVertexShader() = delete;
		D3DVertexShader(ID3D11Device& d3dDevice, const BYTE* shaderByteData, SIZE_T shaderByteSize);
		D3DVertexShader(const D3DVertexShader& other) = delete;
		D3DVertexShader(D3DVertexShader&& other) noexcept;
		D3DVertexShader& operator=(const D3DVertexShader& other) = delete;
		D3DVertexShader& operator=(D3DVertexShader&& other) noexcept;
		~D3DVertexShader();

		virtual const BYTE* getBytePointer() const override;
		virtual SIZE_T getByteSize() const override;
	};

	class D3DPixelShader : public D3DShaderElement{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

		const BYTE* shaderByteData;
		SIZE_T shaderByteSize;

		//FUNCTIONS
	public:
		D3DPixelShader() = delete;
		D3DPixelShader(ID3D11Device& d3dDevice, const BYTE* shaderByteData, SIZE_T shaderByteSize);
		D3DPixelShader(const D3DPixelShader& other) = delete;
		D3DPixelShader(D3DPixelShader&& other) noexcept;
		D3DPixelShader& operator=(const D3DPixelShader& other) = delete;
		D3DPixelShader& operator=(D3DPixelShader&& other) noexcept;
		~D3DPixelShader();

		virtual const BYTE* getBytePointer() const override;
		virtual SIZE_T getByteSize() const override;
	};

	class D3DGeometryShader : public D3DShaderElement{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
		
		const BYTE* shaderByteData;
		SIZE_T shaderByteSize;

		//FUNCTIONS
	public:
		D3DGeometryShader() = delete;
		D3DGeometryShader(ID3D11Device& d3dDevice, const BYTE* shaderByteData, SIZE_T shaderByteSize);
		D3DGeometryShader(const D3DGeometryShader& other) = delete;
		D3DGeometryShader(D3DGeometryShader&& other) noexcept;
		D3DGeometryShader& operator=(const D3DGeometryShader& other) = delete;
		D3DGeometryShader& operator=(D3DGeometryShader&& other) noexcept;
		~D3DGeometryShader();

		virtual const BYTE* getBytePointer() const override;
		virtual SIZE_T getByteSize() const override;
	};
}