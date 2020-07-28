#pragma once

namespace clv::gfx{
	enum class API{
		None,
	#if GARLIC_PLATFORM_WINDOWS
		OpenGL4,
		Direct3D11
	#elif GARLIC_PLATFORM_LINUX
		OpenGL4,
	#elif GARLIC_PLATFORM_MACOS
		Metal1,
	#endif
	};

	enum class ShaderStage{
		Vertex,
		Pixel,
		Geometry
	};

	enum class VertexElementType{
		position2D,
		position3D,
		texture2D,
		normal,
		colour3D,
		jointIds,
		weights,
	};

	struct Viewport{
		int32_t x = 0;
		int32_t y = 0;
		int32_t width = 0;
		int32_t height = 0;
	};

	enum BufferBindingPoint {
		//General
        BBP_Colour                     = 1u,

		//2D
        BBP_2DData                     = 2u,

		//3D
        BBP_CameraMatrices             = 2u,
        BBP_LightData                  = 3u,
        BBP_ViewData                   = 4u,
        BBP_ModelData                  = 5u,
        BBP_MaterialData               = 6u,
        BBP_PointShadowTransform       = 7u,
        BBP_CubeDepthData              = 8u,
        BBP_CurrentLights              = 9u,
        BBP_CurrentFaceIndex           = 10u,
        BBP_DirectionalShadowTransform = 11u,
        BBP_AllDirectionalTransform    = 12u,
        BBP_SkeletalData               = 13u,
	};

	enum TextureBindingPoint{
		TBP_Albedo				= 1u,
		TBP_Specular			= 2u,
		TBP_DirectionalShadow	= 3u,
		TBP_PointShadow			= 4u,
		TBP_None				= 5u
	};
}
