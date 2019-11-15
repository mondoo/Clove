#pragma once

#include "Core/Graphics/PipelineObject.hpp"

#include <d3d11.h>
#include <wrl.h>

struct ID3D11Device;
struct ID3D11InputLayout;

namespace clv::gfx::d3d{
	class D3DPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;

		//FUNCTIONS
	public:
		D3DPipelineObject() = delete;
		D3DPipelineObject(ID3D11Device& d3dDevice, const std::shared_ptr<Shader>& shader);
		D3DPipelineObject(const D3DPipelineObject& other) = delete;
		D3DPipelineObject(D3DPipelineObject&& other);
		D3DPipelineObject& operator=(const D3DPipelineObject& other) = delete;
		D3DPipelineObject& operator=(D3DPipelineObject&& other);
		virtual ~D3DPipelineObject();

		const Microsoft::WRL::ComPtr<ID3D11InputLayout>& getD3DInputLayout() const;

		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;

	private:
		DXGI_FORMAT getDXGIFormatFromType(VertexElementType type);
	};
}