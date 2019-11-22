#include "RenderCommand.hpp"

#if CLV_PLATFORM_WINDOWS || CLV_PLATFORM_LINUX
	#include "Graphics/OpenGL/GL.hpp"
#elif CLV_PLATFORM_WINDOWS
	#include "Graphics/Direct3D/D3D.hpp"
#elif CLV_PLATFORM_MACOS
//TODO
#endif

namespace clv::gfx{
	std::unique_ptr<RenderDevice> RenderCommand::device;
	std::unique_ptr<RenderFactory> RenderCommand::factory;

	void RenderCommand::bindIndexBuffer(const Buffer& buffer){
		device->bindIndexBuffer(buffer);
	}

	void RenderCommand::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		device->bindVertexBuffer(buffer, stride);
	}

	void RenderCommand::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		device->bindShaderResourceBuffer(buffer, shaderType, bindingPoint);
	}

	void RenderCommand::bindPipelineObject(const PipelineObject& pipelineObject){
		device->bindPipelineObject(pipelineObject);
	}

	void RenderCommand::bindTexture(const Texture* texture, const uint32 bindingPoint){
		device->bindTexture(texture, bindingPoint);
	}

	void RenderCommand::bindShader(const Shader& shader){
		device->bindShader(shader);
	}

	void RenderCommand::updateBufferData(Buffer& buffer, const void* data){
		device->updateBufferData(buffer, data);
	}

	void RenderCommand::makeSurfaceCurrent(Surface& surface){
		device->makeSurfaceCurrent(surface);
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

	void RenderCommand::setClearColour(const mth::vec4f& colour){
		device->setClearColour(colour);
	}

	void RenderCommand::setDepthBuffer(bool enabled){
		device->setDepthBuffer(enabled);
	}

	void RenderCommand::setBlendState(bool enabled){
		device->setBlendState(enabled);
	}

	std::shared_ptr<Buffer> RenderCommand::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return factory->createBuffer(descriptor, data);
	}

	std::shared_ptr<Texture> RenderCommand::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return factory->createTexture(descriptor, pathToTexture);
	}

	std::shared_ptr<Texture> RenderCommand::createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP){
		return factory->createTexture(descriptor, data, BPP);
	}

	std::shared_ptr<PipelineObject> RenderCommand::createPipelineObject(const std::shared_ptr<Shader>& shader){
		return factory->createPipelineObject(shader);
	}

	std::shared_ptr<RenderTarget> RenderCommand::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return factory->createRenderTarget(colourTexture, depthStencilTexture);
	}

	std::shared_ptr<Shader> RenderCommand::createShader(const ShaderDescriptor& descriptor){
		return factory->createShader(descriptor);
	}

	std::shared_ptr<Surface> RenderCommand::createSurface(void* windowData){
		return factory->createSurface(windowData);
	}

	void RenderCommand::initialise(gfx::API api){
		switch(api){
			#if CLV_PLATFORM_WINDOWS || CLV_PLATFORM_LINUX
			case API::OpenGL4:
				{
					CLV_LOG_TRACE("Creating OpenGL renderer");
					auto pair = ogl::initialiseOGL();
					device = std::move(pair.first);
					factory = std::move(pair.second);
				}
				break;

			#elif CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				{
					CLV_LOG_TRACE("Creating Direct3D API");
					auto pair = d3d::initialiseD3D();
					device = std::move(pair.first);
					factory = std::move(pair.second);
				}
				break;
				
			#elif CLV_PLATFORM_MACOS
			case API::Metal1:
				{
					//TODO
				}
				break;
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", CLV_FUNCTION_NAME);
				break;
		}
	}
}
