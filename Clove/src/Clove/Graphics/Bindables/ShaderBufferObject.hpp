#pragma once

#include "Clove/Graphics/Bindable.hpp"

namespace clv::gfx{
	enum BufferBindingPoint{ //Will this be needed?
		BBP_CameraMatrices		= 0u,
		BBP_PointLightData		= 1u,
		BBP_ViewData			= 2u,
		BBP_ModelData			= 3u,
		BBP_MaterialData		= 4u,
		BBP_2DData				= 5u,
		BBP_ShadowData			= 6u,
		BBP_CubeDepthData		= 7u,
		BBP_CurrentLights		= 8u,
		BBP_CurrentLightIndex	= 9u,
		BBP_CurrentDepthData	= 10u,
	};

	/*
	Am i going to have to change these to take void* data?
	- I feel like this will turn into the vertex layout equivalent
	*/

	template<typename T>
	class ShaderBufferObject : public Bindable{
		//FUNCTIONS
	public:
		ShaderBufferObject() = default;
		ShaderBufferObject(const ShaderBufferObject& other) = delete;
		ShaderBufferObject(ShaderBufferObject&& other) = default;
		ShaderBufferObject& operator=(const ShaderBufferObject& other) = delete;
		ShaderBufferObject& operator=(ShaderBufferObject&& other) = default;
		virtual ~ShaderBufferObject() = default;

		virtual void update(const T& data) = 0;
	};
}