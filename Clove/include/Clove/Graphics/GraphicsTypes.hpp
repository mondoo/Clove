#pragma once

namespace clv::gfx{
	enum class API{
		None,
	#if CLV_PLATFORM_WINDOWS
		OpenGL4,
		Direct3D11
	#elif CLV_PLATFORM_LINUX
		OpenGL4,
	#elif CLV_PLATFORM_MACOS
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
        BBP_CameraMatrices             = 1u,
        BBP_LightData                  = 2u,
        BBP_ViewData                   = 3u,
        BBP_ModelData                  = 4u,
        BBP_MaterialData               = 5u,
        BBP_2DData                     = 6u,
        BBP_PointShadowTransform       = 7u,
        BBP_CubeDepthData              = 8u,
        BBP_CurrentLights              = 9u,
        BBP_CurrentFaceIndex           = 10u,
        BBP_DirectionalShadowTransform = 11u,
        BBP_Colour                     = 12u,
        BBP_AllDirectionalTransform    = 13u,
        BBP_SkeletalData               = 14u,
	};

	enum TextureBindingPoint{
		TBP_Albedo				= 1u,
		TBP_Specular			= 2u,
		TBP_DirectionalShadow	= 3u,
		TBP_PointShadow			= 4u,
		TBP_None				= 5u
	};
}
