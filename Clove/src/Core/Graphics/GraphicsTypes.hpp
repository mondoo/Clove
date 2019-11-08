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
		Font,
		CubeShadowMap
	};

	enum class TextureStyle{
		Default,
		Cubemap
	};

	enum class TextureUsage{
		Default,
		RenderTarget_Colour,
		RenderTarget_Depth,
		Font
	};

	enum class VertexElementType{
		position2D,
		position3D,
		texture2D,
		normal,
	};

	struct Viewport{
		uint32 x = 0;
		uint32 y = 0;
		uint32 width = 0;
		uint32 height = 0;
	};

	enum BufferBindingPoint{ //Will this be needed?
		BBP_CameraMatrices = 0u,
		BBP_PointLightData = 1u,
		BBP_ViewData = 2u,
		BBP_ModelData = 3u,
		BBP_MaterialData = 4u,
		BBP_2DData = 5u,
		BBP_ShadowData = 6u,
		BBP_CubeDepthData = 7u,
		BBP_CurrentLights = 8u,
		BBP_CurrentFaceIndex = 9u,
		BBP_CurrentDepthData = 10u,
	};
}