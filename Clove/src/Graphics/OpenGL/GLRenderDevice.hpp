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

		virtual void bindIndexBuffer(IndexBuffer& buffer) override;
		virtual void bindVertexBuffer(VertexBuffer& buffer) override;

		virtual void bindShaderResource(ShaderResource& resource) override;

		virtual void bindTexture(Texture& texture) override;

		virtual void bindShader(Shader& shader) override;

		virtual void makeSurfaceCurrent(const Surface& surface) override;

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