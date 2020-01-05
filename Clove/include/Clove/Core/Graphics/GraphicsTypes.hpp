#pragma once

namespace clv::gfx{
	enum class API{
		None,
	#if CLV_PLATFORM_WINDOWS
		OpenGL4,
		DirectX11,
	#elif CLV_PLATFORM_LINUX
		OpenGL4,
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
		int32 x = 0;
		int32 y = 0;
		int32 width = 0;
		int32 height = 0;
	};

	enum BufferBindingPoint{ //Will this be needed?
		BBP_CameraMatrices 		= 1u,
		BBP_PointLightData 		= 2u,
		BBP_ViewData 			= 3u,
		BBP_ModelData 			= 4u,
		BBP_MaterialData 		= 5u,
		BBP_2DData 				= 6u,
		BBP_ShadowData 			= 7u,
		BBP_CubeDepthData 		= 8u,
		BBP_CurrentLights 		= 9u,
		BBP_CurrentFaceIndex 	= 10u,
		BBP_CurrentDepthData 	= 11u,
		BBP_Colour 				= 12u
	};

	enum TextureBindingPoint{
		TBP_Albedo = 1u,
		TBP_Specular = 2u,
		TBP_Shadow = 3u,
		TBP_None = 4u
	};
}
