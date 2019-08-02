#pragma once

namespace clv::gfx{
	enum class API{
		None,
		OpenGL4,
	#if CLV_PLATFORM_WINDOWS
		DirectX11
	#endif
	};

	enum class ShaderType{
		Vertex,
		Pixel,
	};

	enum class ShaderStyle{
		Lit,
		Unlit,
		_2D
	};
}