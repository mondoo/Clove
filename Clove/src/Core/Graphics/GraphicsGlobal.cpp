#include "Clove/Core/Graphics/GraphicsGlobal.hpp"

#include "Clove/Graphics/OpenGL/GL.hpp"
#if CLV_PLATFORM_WINDOWS
	#include "Clove/Graphics/Direct3D/D3D.hpp"
#endif

namespace clv::gfx::global{
	void initialise(gfx::API api){
		switch(api){
			case API::OpenGL4:
				{
					CLV_LOG_TRACE("Creating OpenGL renderer");
					auto pair = ogl::initialiseOGL();
					graphicsDevice = std::move(pair.first);
					graphicsFactory = std::move(pair.second);
					break;
				}

			#if CLV_PLATFORM_WINDOWS
			case API::Direct3D11:
				{
					CLV_LOG_TRACE("Creating Direct3D renderer");
					auto pair = d3d::initialiseD3D();
					graphicsDevice = std::move(pair.first);
					graphicsFactory = std::move(pair.second);
					break;
				}
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", CLV_FUNCTION_NAME);
				break;
		}
	}
}