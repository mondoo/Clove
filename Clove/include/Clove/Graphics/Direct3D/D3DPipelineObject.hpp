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
		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		D3D11_BLEND_DESC blendDesc;

		//FUNCTIONS
	public:
		D3DPipelineObject() = delete;
		D3DPipelineObject(ID3D11Device& d3dDevice, const std::shared_ptr<Shader>& shader);

		D3DPipelineObject(const D3DPipelineObject& other) = delete;
		D3DPipelineObject(D3DPipelineObject&& other);

		D3DPipelineObject& operator=(const D3DPipelineObject& other) = delete;
		D3DPipelineObject& operator=(D3DPipelineObject&& other);

		virtual ~D3DPipelineObject();

		virtual void setBlendState(bool enabled) override;

		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;

		const Microsoft::WRL::ComPtr<ID3D11InputLayout>& getD3DInputLayout() const;
		const D3D11_BLEND_DESC& getD3DBlendDesc() const;

	private:
		DXGI_FORMAT getDXGIFormatFromType(VertexElementType type);
	};
}