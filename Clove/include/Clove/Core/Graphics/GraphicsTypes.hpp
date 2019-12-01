#pragma once

namespace clv::gfx{
	enum class API{
		None,
	#if CLV_PLATFORM_WINDOWS || CLV_PLATFORM_LINUX
		OpenGL4,
	#elif CLV_PLATFORM_WINDOWS
		DirectX11,
	#elif CLV_PLATFORM_MACOS
		Metal1,
	#endif
	};

	enum class ShaderType{
		Vertex,
		Pixel,
		Geometry
	};

	enum class ShaderStyle{
		Lit_3D,
		Unlit_3D,
		Unlit_2D,
		RT,
		Font,
		CubeShadowMap
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
		BBP_Colour = 11u
	};

	enum TextureBindingPoint{
		TBP_Albedo = 1u,
		TBP_Specular = 2u,
		TBP_Shadow = 3u,
		TBP_None = 4u
	};
}
