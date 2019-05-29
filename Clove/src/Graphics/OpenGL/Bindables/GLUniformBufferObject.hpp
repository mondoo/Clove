#pragma once

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	template<typename T>
	class UniformBufferObject : public ShaderBufferObject<T>{
		//VARIABLES
	private:
		unsigned int uboID = 0;

		//FUNCTIONS
	public:
		UniformBufferObject() = delete;
		UniformBufferObject(const UniformBufferObject& other) = delete;
		UniformBufferObject(UniformBufferObject&& other) = default;
		UniformBufferObject& operator=(const UniformBufferObject& other) = delete;
		UniformBufferObject& operator=(UniformBufferObject&& other) = default;
		~UniformBufferObject();

		UniformBufferObject(unsigned int bindingPoint);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void update(const T& data, Renderer& renderer) override;
	};
}

#include "GLUniformBufferObject.inl"