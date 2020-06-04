#include "Clove/Graphics/Graphics.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Clove/Graphics/OpenGL/GL.hpp"
	#include "Clove/Graphics/Direct3D/D3D.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Graphics/OpenGL/GL.hpp"
#elif CLV_PLATFORM_MACOS
	#include "Clove/Graphics/Metal/MTL.hpp"
#endif

namespace clv::gfx{
	std::shared_ptr<GraphicsFactory> initialise(API api) {
		switch(api){
			#if CLV_PLATFORM_WINDOWS
			case API::OpenGL4:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating OpenGL graphics context");
				return ogl::initialiseOGL();

			case API::Direct3D11:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating Direct3D graphics context");
				return d3d::initialiseD3D();

			#elif CLV_PLATFORM_LINUX
			case API::OpenGL4:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating OpenGL graphics context");
				return ogl::initialiseOGL();
				
			#elif CLV_PLATFORM_MACOS
			case API::Metal1:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating Metal graphics context");
				return mtl::initialiseMTL();
			#endif

			default:
				GARLIC_LOG(garlicLogContext, Log::Level::Error, "Default statement hit. Could not initialise RenderAPI: {0}", CLV_FUNCTION_NAME);
				return std::shared_ptr<GraphicsFactory>();
		}
	}
}
