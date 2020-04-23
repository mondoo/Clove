#pragma once

#include "Clove/Graphics/PipelineObject.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl {
	class MTLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		VertexLayout vertexLayout;
		
		MTLRenderPipelineDescriptor* pipelineDescriptor;
		
		CullFace cullFace;
		bool frontFaceCounterClockwise = false;
		
		//FUNCTIONS
	public:
		MTLPipelineObject(std::shared_ptr<GraphicsFactory> factory);
		
		MTLPipelineObject(const MTLPipelineObject& other) = delete;
		MTLPipelineObject(MTLPipelineObject&& other) noexcept;
		
		MTLPipelineObject& operator=(const MTLPipelineObject& other) = delete;
		MTLPipelineObject& operator=(MTLPipelineObject&& other) noexcept;
		
		~MTLPipelineObject();
		
		const std::shared_ptr<GraphicsFactory>& getFactory() const override;
		
		void setVertexShader(const Shader& vertexShader) override;
		void setGeometryShader(const Shader& geometryShader) override;
		void setPixelShader(const Shader& pixelShader) override;
		
		void setBlendState(bool enabled) override;
		void setCullMode(CullFace face, bool frontFaceCounterClockwise) override;
		
		virtual const VertexLayout& getVertexLayout() const override;
		
		MTLRenderPipelineDescriptor* getMTLPipelineStateDescriptor() const;
		
		CullFace getCullFace() const;
		bool isFrontFaceCounterClockWise() const;
		
	private:
		MTLVertexFormat getMTLFormatFromType(VertexElementType type);
	};
}
