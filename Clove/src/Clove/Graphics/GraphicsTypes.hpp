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
		Geometry
	};

	enum class ShaderStyle{ //TODO: Rename to material style
		Lit,
		Unlit,
		_2D,
		RT,
		CubeShadowMap
	};

	enum class TextureStyle{
		Default,
		Cubemap
	};

	enum class TextureUsage{
		Default,
		RenderTarget_Colour,
		RenderTarget_Depth
	};

	enum class VertexElementType{
		position2D,
		position3D,
		texture2D,
		normal,
	};
}