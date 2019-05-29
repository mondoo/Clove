#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
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