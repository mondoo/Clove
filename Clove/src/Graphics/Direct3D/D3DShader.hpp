#pragma once

#include "Core/Graphics/Shader.hpp"

#include <wrl.h>
#include <d3d.h>

struct ID3D11Device;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;

namespace clv::gfx::d3d::_11{
	class D3DShader : public Shader{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> d3dVertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> d3dGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> d3dPixelShader;

		const BYTE* vertexByteData = nullptr;
		SIZE_T vertexByteSize = 0u;

		ShaderDescriptor descriptor;
		ShaderReflectionData reflectionData;

		//FUNCTIONS	
	public:
		D3DShader() = delete;
		D3DShader(ID3D11Device& d3dDevice, const ShaderDescriptor& descriptor);
		D3DShader(const D3DShader& other) = delete;
		D3DShader(D3DShader&& other) noexcept;
		D3DShader& operator=(const D3DShader& other) = delete;
		D3DShader& operator=(D3DShader&& other) noexcept;
		virtual ~D3DShader();

		virtual const ShaderDescriptor& getDescriptor() const override;
		virtual ShaderReflectionData getReflectionData() const override;

		const Microsoft::WRL::ComPtr<ID3D11VertexShader>& getD3DVertexShader() const;
		const Microsoft::WRL::ComPtr<ID3D11GeometryShader>& getD3DGeometryShader() const;
		const Microsoft::WRL::ComPtr<ID3D11PixelShader>& getD3DPixelShader() const;

	private:
		void initialise(ID3D11Device& d3dDevice, ShaderStyle style);
	};
}