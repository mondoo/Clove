#include "clvpch.hpp"
#include "Context.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/OpenGL-4/WGLContext.hpp"
#include "Graphics/DirectX-11/DXContext.hpp"
#elif CLV_PLATFORM_LINUX
#include "Graphics/OpenGL-4/GLXContext.hpp"
#endif

namespace clv::gfx{
	Context::Context(Context&& other) noexcept = default;

	Context& Context::operator=(Context&& other) noexcept = default;

	Context::~Context() = default;

	std::unique_ptr<Context> Context::createContext(void* windowData, API api){
		switch(api){
			case API::OpenGL4:
			#if CLV_PLATFORM_WINDOWS
				return std::make_unique<WGLContext>(windowData);
			#elif CLV_PLATFORM_LINUX
				return std::make_unique<GLXContext>(windowData);
			#endif

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				return std::make_unique<DXContext>(windowData);
			#endif
			
			default:
				CLV_LOG_ERROR("Default statement hit. No context initialised {0}", CLV_FUNCTION_NAME);
				return std::unique_ptr<Context>();
		}

		return std::unique_ptr<Context>();
	}
}