#pragma once

#include "Clove/Graphics/Core/PipelineObject.hpp"

#include <d3d11.h>
#include <wrl.h>

struct ID3D11Device;
struct ID3D11InputLayout;

namespace clv::gfx::d3d{
	class D3DPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		VertexLayout vertexLayout;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> d3dVertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> d3dGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> d3dPixelShader;

		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		D3D11_BLEND_DESC blendDesc;
		D3D11_RASTERIZER_DESC rasteriserDesc;

		//FUNCTIONS
	public:
		D3DPipelineObject() = delete;
		D3DPipelineObject(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice);

		D3DPipelineObject(const D3DPipelineObject& other) = delete;
		D3DPipelineObject(D3DPipelineObject&& other);

		D3DPipelineObject& operator=(const D3DPipelineObject& other) = delete;
		D3DPipelineObject& operator=(D3DPipelineObject&& other);

		virtual ~D3DPipelineObject();

		virtual void setVertexShader(const Shader& vertexShader) override;
		virtual void setGeometryShader(const Shader& geometryShader) override;
		virtual void setPixelShader(const Shader& pixelShader) override;

		virtual void setBlendState(bool enabled) override;
		virtual void setCullMode(CullFace face, bool frontFaceCounterClockwise) override;

		virtual const VertexLayout& getVertexLayout() const override;

		const Microsoft::WRL::ComPtr<ID3D11VertexShader>& getD3DVertexShader() const;
		const Microsoft::WRL::ComPtr<ID3D11GeometryShader>& getD3DGeometryShader() const;
		const Microsoft::WRL::ComPtr<ID3D11PixelShader>& getD3DPixelShader() const;

		const Microsoft::WRL::ComPtr<ID3D11InputLayout>& getD3DInputLayout() const;
		const D3D11_BLEND_DESC& getD3DBlendDesc() const;
		const D3D11_RASTERIZER_DESC& getD3DRasterDesc() const;
	};
}