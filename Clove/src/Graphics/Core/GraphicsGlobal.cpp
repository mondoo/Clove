#include "Clove/Graphics/Core/GraphicsGlobal.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Clove/Graphics/OpenGL/GL.hpp"
	#include "Clove/Graphics/Direct3D/D3D.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Graphics/OpenGL/GL.hpp"
#elif CLV_PLATFORM_MACOS
	#include "Clove/Graphics/Metal/MTL.hpp"
#endif

namespace clv::gfx::global{
	void initialise(gfx::API api){
		switch(api){
			#if CLV_PLATFORM_WINDOWS
			case API::OpenGL4:
				{
					CLV_LOG_TRACE("Creating OpenGL renderer");
					auto pair = ogl::initialiseOGL();
					graphicsDevice = std::move(pair.first);
					graphicsFactory = std::move(pair.second);
				}
				break;

			case API::Direct3D11:
				{
					CLV_LOG_TRACE("Creating Direct3D renderer");
					auto pair = d3d::initialiseD3D();
					graphicsDevice = std::move(pair.first);
					graphicsFactory = std::move(pair.second);
				}
				break;

			#elif CLV_PLATFORM_LINUX
			case API::OpenGL4:
				{
					CLV_LOG_TRACE("Creating OpenGL renderer");
					auto pair = ogl::initialiseOGL();
					graphicsDevice = std::move(pair.first);
					graphicsFactory = std::move(pair.second);
				}
				break;
				
			#elif CLV_PLATFORM_MACOS
			case API::Metal1:
				{
					CLV_LOG_TRACE("Creating Metal renderer");
					auto pair = mtl::initialiseMTL();
					graphicsDevice = std::move(pair.first);
					graphicsFactory = std::move(pair.second);
				}
				break;
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", CLV_FUNCTION_NAME);
				break;
		}
	}
}
