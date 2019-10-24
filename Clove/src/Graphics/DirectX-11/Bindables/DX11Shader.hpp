#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;

namespace clv::gfx{
	class Renderer;

	class DX11ShaderElement;
	class DX11VertexShader;

	class DX11Shader : public Shader{
		//VARIABLES
	private:
		std::unordered_map<ShaderType, std::unique_ptr<DX11ShaderElement>> shaders;
		DX11VertexShader* vertexShader = nullptr;

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
	};

	//TODO: move all of this out of this header

	class DX11ShaderElement : public Bindable{
		//FUNCTIONS	
	public:
		DX11ShaderElement();
		DX11ShaderElement(const DX11ShaderElement& other) = delete;
		DX11ShaderElement(DX11ShaderElement&& other) noexcept;
		DX11ShaderElement& operator=(const DX11ShaderElement& other) = delete;
		DX11ShaderElement& operator=(DX11ShaderElement&& other) noexcept;
		virtual ~DX11ShaderElement();

		virtual const BYTE* getBytePointer() const = 0;
		virtual SIZE_T getByteSize() const = 0;
	};

	class DX11VertexShader : public DX11ShaderElement{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		
		const BYTE* shaderByteData;
		SIZE_T shaderByteSize;

		//FUNCTIONS
	public:
		DX11VertexShader() = delete;
		DX11VertexShader(const DX11VertexShader& other) = delete;
		DX11VertexShader(DX11VertexShader&& other) noexcept;
		DX11VertexShader& operator=(const DX11VertexShader& other) = delete;
		DX11VertexShader& operator=(DX11VertexShader&& other) noexcept;
		~DX11VertexShader();

		DX11VertexShader(const BYTE* shaderByteData, SIZE_T shaderByteSize);

		virtual void bind() override;

		virtual const BYTE* getBytePointer() const override;
		virtual SIZE_T getByteSize() const override;
	};

	class DX11PixelShader : public DX11ShaderElement{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

		const BYTE* shaderByteData;
		SIZE_T shaderByteSize;

		//FUNCTIONS
	public:
		DX11PixelShader() = delete;
		DX11PixelShader(const DX11PixelShader& other) = delete;
		DX11PixelShader(DX11PixelShader&& other) noexcept;
		DX11PixelShader& operator=(const DX11PixelShader& other) = delete;
		DX11PixelShader& operator=(DX11PixelShader&& other) noexcept;
		~DX11PixelShader();

		DX11PixelShader(const BYTE* shaderByteData, SIZE_T shaderByteSize);

		virtual void bind() override;

		virtual const BYTE* getBytePointer() const override;
		virtual SIZE_T getByteSize() const override;
	};

	class DX11GeometryShader : public DX11ShaderElement{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
		
		const BYTE* shaderByteData;
		SIZE_T shaderByteSize;

		//FUNCTIONS
	public:
		DX11GeometryShader() = delete;
		DX11GeometryShader(const DX11GeometryShader& other) = delete;
		DX11GeometryShader(DX11GeometryShader&& other) noexcept;
		DX11GeometryShader& operator=(const DX11GeometryShader& other) = delete;
		DX11GeometryShader& operator=(DX11GeometryShader&& other) noexcept;
		~DX11GeometryShader();

		DX11GeometryShader(const BYTE* shaderByteData, SIZE_T shaderByteSize);

		virtual void bind() override;

		virtual const BYTE* getBytePointer() const override;
		virtual SIZE_T getByteSize() const override;
	};
}