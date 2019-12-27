#pragma once

#include "Clove/Core/Graphics/PipelineObject.hpp"

namespace clv::gfx::mtl {
	class MTLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		id<MTLRenderPipelineState> pipelineState;
		
		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;
		
		//FUNCTIONS
	public:
		MTLPipelineObject() = delete;
		MTLPipelineObject(id<MTLDevice> mtlDevice, const std::shared_ptr<Shader>& shader);
		
		MTLPipelineObject(const MTLPipelineObject& other) = delete;
		MTLPipelineObject(MTLPipelineObject&& other) noexcept;
		
		MTLPipelineObject& operator=(const MTLPipelineObject& other) = delete;
		MTLPipelineObject& operator=(MTLPipelineObject&& other) noexcept;
		
		virtual ~MTLPipelineObject();
		
		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;
		
	private:
		MTLVertexFormat getMTLFormatFromType(VertexElementType type);
	};
}
