#include "Clove/Core/Graphics/GraphicsGlobal.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Clove/Graphics/OpenGL/GL.hpp"
	#include "Clove/Graphics/Direct3D/D3D.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Graphics/OpenGL/GL.hpp"
#elif CLV_PLATFORM_MACOS
	//TODO
#endif

namespace clv::gfx::global{
	void initialise(gfx::API api){
		switch(api){
			#if CLV_PLATFORM_WINDOWS
			case API::OpenGL4:
				{
					CLV_LOG_TRACE("Creating OpenGL renderer");
					auto pair = ogl::initialiseOGL();
					device = std::move(pair.first);
					factory = std::move(pair.second);
				}
				break;

			case API::DirectX11:
				{
					CLV_LOG_TRACE("Creating Direct3D API");
					auto pair = d3d::initialiseD3D();
					device = std::move(pair.first);
					factory = std::move(pair.second);
				}
				break;

			#elif CLV_PLATFORM_LINUX
			case API::OpenGL4:
				{
					CLV_LOG_TRACE("Creating OpenGL renderer");
					auto pair = ogl::initialiseOGL();
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
