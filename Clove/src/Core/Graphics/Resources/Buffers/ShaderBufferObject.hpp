#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

namespace clv::gfx{
	struct ShaderBufferObjectDescriptor{
		//TODO
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
	template<typename T>
	class ShaderBufferObject : public Buffer{
		//FUNCTIONS
	public:
		ShaderBufferObject() = default;
		ShaderBufferObject(const ShaderBufferObject& other) = delete;
		ShaderBufferObject(ShaderBufferObject&& other) = default;
		ShaderBufferObject& operator=(const ShaderBufferObject& other) = delete;
		ShaderBufferObject& operator=(ShaderBufferObject&& other) = default;
		virtual ~ShaderBufferObject() = default;

		virtual void update(T&& data) = 0;
	};
}