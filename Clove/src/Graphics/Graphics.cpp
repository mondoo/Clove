#include "Clove/Graphics/Graphics.hpp"

#if GARLIC_PLATFORM_WINDOWS
	#include "Clove/Graphics/OpenGL/GL.hpp"
	#include "Clove/Graphics/Direct3D/D3D.hpp"
#elif GARLIC_PLATFORM_LINUX
	#include "Clove/Graphics/OpenGL/GL.hpp"
#elif GARLIC_PLATFORM_MACOS
	#include "Clove/Graphics/Metal/MTL.hpp"
#endif

#include <Root/Definitions.hpp>

namespace clv::gfx{
	std::shared_ptr<GraphicsFactory> initialise(API api) {
		switch(api){
			#if GARLIC_PLATFORM_WINDOWS
			case API::OpenGL4:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating OpenGL  renderer backend");
				return ogl::initialiseOGL();

			case API::Direct3D11:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating Direct3D  renderer backend");
				return d3d::initialiseD3D();

			#elif GARLIC_PLATFORM_LINUX
			case API::OpenGL4:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating OpenGL  renderer backend");
				return ogl::initialiseOGL();

			case API::Vulkan:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating Vulkan renderer backend");
				return nullptr;
				
			#elif GARLIC_PLATFORM_MACOS
			case API::Metal1:
				GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Creating Metal  renderer backend");
				return mtl::initialiseMTL();
			#endif

			default:
				GARLIC_LOG(garlicLogContext, Log::Level::Error, "Default statement hit. Could not initialise RenderAPI: {0}", GARLIC_FUNCTION_NAME);
				return std::shared_ptr<GraphicsFactory>();
		}
	}
}
