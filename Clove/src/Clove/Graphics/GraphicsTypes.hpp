#pragma once

namespace clv::gfx{
	enum class API{
		OpenGL4,
	#if CLV_PLATFORM_WINDOWS
		DirectX11
	#endif
	};

	//TODO: These need adjusting
	enum class ShaderType{
		Vertex,
		Pixel,
		Vertex2D,
		Pixel2D
	};
}