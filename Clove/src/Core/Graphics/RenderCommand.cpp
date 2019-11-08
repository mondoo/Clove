#include "RenderCommand.hpp"

#include "Core/Graphics/RenderDevice.hpp"
#include "Core/Graphics/Resources/Buffers/IndexBuffer.hpp"
#include "Core/Graphics/Resources/Buffers/VertexBuffer.hpp"
#include "Core/Graphics/Resources/ShaderResource.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/Shader.hpp"
#include "Core/Graphics/RenderTarget.hpp"

//#include "Graphics/OpenGL-4/GL4RenderAPI.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/Direct3D/D3D.hpp"
#endif

namespace clv::gfx{
	std::unique_ptr<RenderDevice> RenderCommand::device;

	void RenderCommand::bindIndexBuffer(IndexBuffer& buffer){
		device->bindIndexBuffer(buffer);
	}

	void RenderCommand::bindVertexBuffer(VertexBuffer& buffer){
		device->bindVertexBuffer(buffer);
	}

	void RenderCommand::bindShaderResource(ShaderResource& resource){
		device->bindShaderResource(resource);
	}

	void RenderCommand::bindTexture(Texture& texture){
		device->bindTexture(texture);
	}

	void RenderCommand::bindShader(Shader& shader){
		device->bindShader(shader);
	}

	void RenderCommand::setDefaultRenderTarget(RenderTarget& renderTarget){
		device->setDefaultRenderTarget(renderTarget);
	}

	void RenderCommand::setRenderTarget(RenderTarget& renderTarget){
		device->setRenderTarget(renderTarget);
	}

	void RenderCommand::resetRenderTargetToDefault(){
		device->resetRenderTargetToDefault();
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

	void RenderCommand::removeCurrentGeometryShader(){
		device->removeCurrentGeometryShader();
	}

	void RenderCommand::removeTextureAtSlot(uint32 slot){
		device->removeTextureAtSlot(slot);
	}

	std::shared_ptr<IndexBuffer> RenderCommand::createIndexBuffer(const IndexBufferDescriptor& descriptor){
		return factory->createIndexBuffer(descriptor);
	}

	std::shared_ptr<VertexBuffer> RenderCommand::createVertexBuffer(const VertexBufferDescriptor& descriptor){
		return factory->createVertexBuffer(descriptor);
	}

	std::shared_ptr<ShaderResource> RenderCommand::createShaderResource(const ShaderResourceDescriptor& descriptor){
		return factory->createShaderResource(descriptor);
	}

	std::shared_ptr<Texture> RenderCommand::createTexture(const TextureDescriptor& descriptor){
		return factory->createTexture(descriptor);
	}

	std::shared_ptr<Shader> RenderCommand::createShader(const ShaderDescriptor& descriptor){
		return factory->createShader(descriptor);
	}

	std::shared_ptr<Surface> RenderCommand::createSurface(){
		return factory->createSurface();
	}

	void RenderCommand::initialise(gfx::API api){
		switch(api){
			case API::OpenGL4:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				//renderAPI = std::make_unique<GL4RenderAPI>(context);
				//break;

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				{
					CLV_LOG_TRACE("Creating Direct3D API");
					auto pair = d3d::_11::initialiseD3D();
					device = std::move(pair.first);
					factory = std::move(pair.second);
					break;
				}
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", CLV_FUNCTION_NAME);
				break;
		}
	}

	void RenderCommand::initialiseRenderDevice(){
	}
}