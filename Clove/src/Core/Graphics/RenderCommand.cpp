#include "RenderCommand.hpp"

#include "Core/Graphics/RenderDevice.hpp"
#include "Core/Graphics/Resources/Buffers/IndexBuffer.hpp"
#include "Core/Graphics/Resources/Buffers/VertexBuffer.hpp"
#include "Core/Graphics/Resources/ShaderResource.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/RenderTarget.hpp"
#include "Core/Graphics/Viewport.hpp"

//#include "Graphics/OpenGL-4/GL4RenderAPI.hpp"
//#if CLV_PLATFORM_WINDOWS
//#include "Graphics/DirectX-11/DX11RenderAPI.hpp"
//#endif

namespace clv::gfx{
	std::unique_ptr<RenderDevice> RenderCommand::device;

	void RenderCommand::bindIndexBuffer(IndexBuffer& buffer){
		device->bindIndexBuffer(buffer);
	}

	void RenderCommand::bindVertexBuffer(VertexBuffer& buffer){
		device->bindVertexBuffer(buffer);
	}

	void RenderCommand::bindShaderResource(ShaderResource& resource, uint32 bindingPoint){
		device->bindShaderResource(resource, bindingPoint);
	}

	void RenderCommand::bindTexture(Texture& texture, uint32 bindingPoint){
		device->bindTexture(texture, bindingPoint);
	}

	void RenderCommand::bindShader(Shader& shader){
		device->bindShader(shader);
	}

	void RenderCommand::setRenderTarget(RenderTarget& renderTarget){
		device->setRenderTarget(renderTarget);
	}

	void RenderCommand::setViewport(const Viewport& viewport){
		device->setViewport(viewport);
	}

	void RenderCommand::clear(){
		device->clear();
	}

	void RenderCommand::drawIndexed(const uint32 count){
		device->drawIndexed(count);
	}

	void RenderCommand::setClearColour(const math::Vector4f& colour){
		device->setClearColour(colour);
	}

	void RenderCommand::setDepthBuffer(bool enabled){
		device->setDepthBuffer(enabled);
	}

	void RenderCommand::setBlendState(bool enabled){
		device->setBlendState(enabled);
	}

	std::unique_ptr<IndexBuffer> RenderCommand::createIndexBuffer(const IndexBufferDescriptor& descriptor){
		return factory->createIndexBuffer(descriptor);
	}

	std::unique_ptr<VertexBuffer> RenderCommand::createVertexBuffer(const VertexBufferDescriptor& descriptor){
		return factory->createVertexBuffer(descriptor);
	}

	std::unique_ptr<ShaderResource> RenderCommand::createShaderResource(const ShaderResourceDescriptor& descriptor){
		return factory->createShaderResource(descriptor);
	}

	std::unique_ptr<Texture> RenderCommand::createTexture(const TextureDescriptor& descriptor){
		return factory->createTexture(descriptor);
	}

	std::unique_ptr<Surface> RenderCommand::createSurface(){
		return factory->createSurface();
	}

	std::unique_ptr<Viewport> RenderCommand::createViewport(){
		return factory->createViewport();
	}

	void RenderCommand::initialiseRenderDevice(){
		/*switch(context.getAPI()){
			case API::OpenGL4:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				renderAPI = std::make_unique<GL4RenderAPI>(context);
				break;

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				CLV_LOG_TRACE("Creating DirectX11 renderer");
				renderAPI = std::make_unique<DX11RenderAPI>(context);
				break;
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", CLV_FUNCTION_NAME);
				break;
		}*/
	}
}