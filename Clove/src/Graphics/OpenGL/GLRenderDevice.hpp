#pragma once

#include "Core/Graphics/RenderDevice.hpp"

namespace clv::gfx::ogl{
	class GLRenderDevice : public RenderDevice{
		//FUNCTIONS
	public:
		GLRenderDevice();
		GLRenderDevice(const GLRenderDevice& other) = delete;
		GLRenderDevice(GLRenderDevice&& other) noexcept = delete;
		GLRenderDevice& operator=(const GLRenderDevice& other) = delete;
		GLRenderDevice& operator=(GLRenderDevice&& other) noexcept = delete;
		~GLRenderDevice();

		virtual void bindIndexBuffer(const Buffer& buffer) override;
		virtual void bindVertexBuffer(const Buffer& buffer) override;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint) override;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) override;
		virtual void bindTexture(const Texture& texture, const uint32 bindingPoint) override;
		virtual void bindShader(const Shader& shader) override;

		virtual void updateBufferData(Buffer& buffer, void* data) override;

		virtual void makeSurfaceCurrent(Surface& surface) override;

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		virtual void setRenderTarget(RenderTarget& renderTarget) override;
		virtual void resetRenderTargetToDefault() override;
		//

		virtual void setViewport(const Viewport& viewport) override;

		virtual void clear() override;
		virtual void drawIndexed(const uint32 count) override;

		virtual void setClearColour(const math::Vector4f& colour) override;
		virtual void setDepthBuffer(bool enabled) override;
		virtual void setBlendState(bool enabled) override;

		//TEMP: Need an 'unbind' function or to refactor the bindable system
		virtual void removeCurrentGeometryShader() override;
		virtual void removeTextureAtSlot(uint32 slot) override;
		//
	};
}