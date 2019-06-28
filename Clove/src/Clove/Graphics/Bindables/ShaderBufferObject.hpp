#pragma once

#include "Clove/Graphics/Bindable.hpp"

/*
rename this to GlobalPipelineObject?
I think the SBO should be for well basically the Uniforms as it's tied to a specific shader
we need another name for the cbufs and ubos

I think we'll need to rename the gl and dx counter parts to be the same name as this class though

The problem I think is that the blood uniforms are name dependant which can change every shader
So we either need a bp (binding point) to name or a name to bp somewhere.
Unless there is a way we can set this when binding the shader???

I think I'm over thinking this, couldn't we just do both? give it a name and binding point?
*/

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