#include "GLRenderFactory.hpp"

#include "Graphics/OpenGL/Resources/GLBuffer.hpp"
#include "Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Graphics/OpenGL/GLRenderTarget.hpp"
#include "Graphics/OpenGL/GLShader.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/OpenGL/WGLSurface.hpp"
#elif CLV_PLATFORM_LINUX
#include "Graphics/OpenGL/GLXSurface.hpp"
#elif CLV_PLATFORM_MACOS
#include "Graphics/OpenGL/CGLSurface.hpp"
#endif

namespace clv::gfx::ogl{
	GLRenderFactory::GLRenderFactory() = default;

	GLRenderFactory::~GLRenderFactory() = default;

	std::shared_ptr<Buffer> GLRenderFactory::createBuffer(const BufferDescriptor& descriptor, void* data){
		return std::make_unique<GLBuffer>(descriptor, data);
	}

	std::shared_ptr<Texture> GLRenderFactory::createTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture){
		return std::make_unique<GLTexture>(descriptor, pathToTexture);
	}

	std::shared_ptr<Texture> GLRenderFactory::createTexture(const TextureDescriptor& descriptor, void* data, int32 BPP){
		return std::make_unique<GLTexture>(descriptor, data, BPP);
	}

	std::shared_ptr<RenderTarget> GLRenderFactory::createRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		return std::make_unique<GLRenderTarget>(colourTexture, depthStencilTexture);
	}

	std::shared_ptr<Shader> GLRenderFactory::createShader(const ShaderDescriptor& descriptor){
		return std::make_unique<GLShader>(descriptor);
	}

	std::shared_ptr<Surface> GLRenderFactory::createSurface(){
	#if CLV_PLATFORM_WINDOWS
		return std::make_unique<WGLSurface>();
	#elif CLV_PLATFORM_LINUX
		return std::make_unique<GLXSurface>();
	#elif CLV_PLATFORM_MACOS
		return std::make_unique<CGLSurface>();
	#endif
	}
}