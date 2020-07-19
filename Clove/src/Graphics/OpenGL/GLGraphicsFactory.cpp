#include "Clove/Graphics/OpenGL/GLGraphicsFactory.hpp"

#include "Clove/Graphics/OpenGL/GLBuffer.hpp"
#include "Clove/Graphics/OpenGL/GLTexture.hpp"
#include "Clove/Graphics/OpenGL/GLCommandBuffer.hpp"
#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"
#include "Clove/Graphics/OpenGL/GLShader.hpp"
#if GARLIC_PLATFORM_WINDOWS
	#include "Clove/Graphics/OpenGL/WGLSurface.hpp"
#elif GARLIC_PLATFORM_LINUX
	#include "Clove/Graphics/OpenGL/GLXSurface.hpp"
#endif

namespace clv::gfx::ogl{
	GLGraphicsFactory::GLGraphicsFactory() = default;

	GLGraphicsFactory::~GLGraphicsFactory() = default;

	std::shared_ptr<CommandBuffer> GLGraphicsFactory::createCommandBuffer(){
		return std::make_shared<GLCommandBuffer>(shared_from_this());
	}

	std::shared_ptr<Buffer> GLGraphicsFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<GLBuffer>(shared_from_this(), descriptor, data);
	}

	std::shared_ptr<Texture> GLGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<GLTexture>(shared_from_this(), descriptor, pathToTexture);
	}

	std::shared_ptr<Texture> GLGraphicsFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP){
		return std::make_shared<GLTexture>(shared_from_this(), descriptor, data, BPP);
	}

	std::shared_ptr<PipelineObject> GLGraphicsFactory::createPipelineObject(){
		return std::make_shared<GLPipelineObject>(shared_from_this());
	}

	std::shared_ptr<RenderTarget> GLGraphicsFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_shared<GLRenderTarget>(shared_from_this(), colourTexture, depthStencilTexture);
	}

	std::shared_ptr<Shader> GLGraphicsFactory::createShader(ShaderDescriptor descriptor, std::string_view pathToShader) {
		return std::make_shared<GLShader>(shared_from_this(), descriptor, pathToShader);
	}

	std::shared_ptr<Shader> GLGraphicsFactory::createShader(ShaderDescriptor descriptor, const char* bytes, const std::size_t size) {
		return std::make_shared<GLShader>(shared_from_this(), descriptor, bytes, size);
	}

	std::shared_ptr<Surface> GLGraphicsFactory::createSurface(void* windowData){
	#if GARLIC_PLATFORM_WINDOWS
		return std::make_shared<WGLSurface>(shared_from_this(), windowData);
	#elif GARLIC_PLATFORM_LINUX
		return std::make_shared<GLXSurface>(shared_from_this(), windowData);
	#endif
	}
}
