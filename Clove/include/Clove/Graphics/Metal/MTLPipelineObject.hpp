#pragma once

#include "Clove/Graphics/Core/PipelineObject.hpp"

namespace clv::gfx::mtl {
	class MTLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;
		
		MTLRenderPipelineDescriptor* pipelineDescriptor;
		id<MTLRenderPipelineState> pipelineState;
		
		CullFace cullFace;
		bool frontFaceCounterClockwise = false;
		
		//FUNCTIONS
	public:
		MTLPipelineObject() = delete;
		MTLPipelineObject(id<MTLDevice> mtlDevice, const std::shared_ptr<Shader>& shader);
		
		MTLPipelineObject(const MTLPipelineObject& other) = delete;
		MTLPipelineObject(MTLPipelineObject&& other) noexcept;
		
		MTLPipelineObject& operator=(const MTLPipelineObject& other) = delete;
		MTLPipelineObject& operator=(MTLPipelineObject&& other) noexcept;
		
		virtual ~MTLPipelineObject();
		
		virtual void setBlendState(bool enabled) override;
		virtual void setCullMode(CullFace face, bool frontFaceCounterClockwise) override;
		
		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;
		
		id<MTLRenderPipelineState> getMTLPipelineState() const;
		
		CullFace getCullFace() const;
		bool isFrontFaceCounterClockWise() const;
		
	private:
		MTLVertexFormat getMTLFormatFromType(VertexElementType type);
	};
}
