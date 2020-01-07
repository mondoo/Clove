#include "Clove/Graphics/OpenGL/GLRenderFactory.hpp"

#include "Clove/Graphics/OpenGL/Resources/GLBuffer.hpp"
#include "Clove/Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"
#include "Clove/Graphics/OpenGL/GLShader.hpp"
#if CLV_PLATFORM_WINDOWS
	#include "Clove/Graphics/OpenGL/WGLSurface.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Graphics/OpenGL/GLXSurface.hpp"
#endif

namespace clv::gfx::ogl{
	GLRenderFactory::GLRenderFactory() = default;

	GLRenderFactory::~GLRenderFactory() = default;

	std::shared_ptr<Buffer> GLRenderFactory::createBuffer(const BufferDescriptor& descriptor, const void* data){
		return std::make_shared<GLBuffer>(descriptor, data);
	}

	std::shared_ptr<Texture> GLRenderFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_shared<GLTexture>(descriptor, pathToTexture);
	}

	std::shared_ptr<Texture> GLRenderFactory::createTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP){
		return std::make_shared<GLTexture>(descriptor, data, BPP);
	}

	std::shared_ptr<PipelineObject> GLRenderFactory::createPipelineObject(const std::shared_ptr<Shader>& shader){
		return std::make_shared<GLPipelineObject>(shader);
	}

	std::shared_ptr<RenderTarget> GLRenderFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_shared<GLRenderTarget>(colourTexture, depthStencilTexture);
	}

	std::shared_ptr<Shader> GLRenderFactory::createShader(const ShaderDescriptor& descriptor){
		return std::make_shared<GLShader>(descriptor);
	}

	std::shared_ptr<Surface> GLRenderFactory::createSurface(void* windowData){
	#if CLV_PLATFORM_WINDOWS
		return std::make_shared<WGLSurface>(windowData);
	#elif CLV_PLATFORM_LINUX
		return std::make_shared<GLXSurface>(windowData);
	#endif
	}
}
