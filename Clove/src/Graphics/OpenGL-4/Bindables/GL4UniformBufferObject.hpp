#pragma once

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	//TODO: Rename when base class is figure out

	template<typename T>
	class GL4UniformBufferObject : public ShaderBufferObject<T>{
		//VARIABLES
	private:
		unsigned int uboID = 0;

		//FUNCTIONS
	public:
		GL4UniformBufferObject() = delete;
		GL4UniformBufferObject(const GL4UniformBufferObject& other) = delete;
		GL4UniformBufferObject(GL4UniformBufferObject&& other) = default;
		GL4UniformBufferObject& operator=(const GL4UniformBufferObject& other) = delete;
		GL4UniformBufferObject& operator=(GL4UniformBufferObject&& other) = default;
		~GL4UniformBufferObject();

		GL4UniformBufferObject(unsigned int bindingPoint);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void update(const T& data, Renderer& renderer) override;
	};
}

#include "GL4UniformBufferObject.inl"