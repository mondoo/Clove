#pragma once

namespace clv::gfx{
	enum class API{
		OpenGL4,
	#if CLV_PLATFORM_WINDOWS
		DirectX11
	#endif
	};

	enum class ShaderTypes{
		Vertex,
		Pixel,
	};
}