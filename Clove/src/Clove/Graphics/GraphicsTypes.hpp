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

	enum class ShaderStyle{ //TODO: Rename to material style
		Lit,
		Unlit,
		_2D,
		RT
	};

	enum class TextureUsage{
		Default,
		RenderTarget
	};

	enum class VertexElementType{
		position2D,
		position3D,
		texture2D,
		normal,
	};
}