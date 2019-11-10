#pragma once

#include "Core/Graphics/PipelineObject.hpp"

namespace clv::gfx::d3d::_11{
	class D3DPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;

		//FUNCTIONS
	public:
		D3DPipelineObject() = delete;
		D3DPipelineObject(const std::shared_ptr<Shader>& shader);
		D3DPipelineObject(const D3DPipelineObject& other) = delete;
		D3DPipelineObject(D3DPipelineObject&& other);
		D3DPipelineObject& operator=(const D3DPipelineObject& other) = delete;
		D3DPipelineObject& operator=(D3DPipelineObject&& other);
		~D3DPipelineObject();

		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;
	};
}