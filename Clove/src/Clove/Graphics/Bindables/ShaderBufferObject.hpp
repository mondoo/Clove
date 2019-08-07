#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/MaterialData.hpp"

namespace clv::gfx{
	enum BufferBindingPoint{
		BBP_CameraMatrices	= 0u,
		BBP_PointLightData	= 1u,
		BBP_ViewData		= 2u,
		BBP_ModelData		= 3u,
		BBP_MaterialData	= 4u,
		BBP_2DData			= 5u
	};

	class ShaderBufferObject : public Bindable{
		//FUNCTIONS
	public:
		ShaderBufferObject();
		ShaderBufferObject(const ShaderBufferObject& other) = delete;
		ShaderBufferObject(ShaderBufferObject&& other);
		ShaderBufferObject& operator=(const ShaderBufferObject& other) = delete;
		ShaderBufferObject& operator=(ShaderBufferObject&& other);
		virtual ~ShaderBufferObject();

		virtual void update(const MaterialData& data) = 0;
	};
}