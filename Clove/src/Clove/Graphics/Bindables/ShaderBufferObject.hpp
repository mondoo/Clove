#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
	enum BufferBindingPoint{
		BBP_CameraMatrices	= 0u,
		BBP_PointLightData	= 1u,
		BBP_ViewData		= 2u,
		BBP_ModelData		= 3u,
		BBP_MaterialData	= 4u
	};

	template<typename T>
	class ShaderBufferObject : public Bindable{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		ShaderBufferObject() = default;
		ShaderBufferObject(const ShaderBufferObject& other) = delete;
		ShaderBufferObject(ShaderBufferObject&& other) = default;
		ShaderBufferObject& operator=(const ShaderBufferObject& other) = delete;
		ShaderBufferObject& operator=(ShaderBufferObject&& other) = default;
		virtual ~ShaderBufferObject() = default;

		virtual void update(const T& data, Renderer& renderer) = 0;
	};
}

#include "ShaderBufferObject.inl"