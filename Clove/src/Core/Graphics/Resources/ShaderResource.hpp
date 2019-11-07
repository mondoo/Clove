#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

namespace clv::gfx{
	struct ShaderResourceDescriptor{
		uint32 bufferSize = 0;
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

namespace clv::gfx{
	class ShaderResource : public Buffer{
		//FUNCTIONS
	public:
		virtual void setData(void* data) = 0;
	};
}