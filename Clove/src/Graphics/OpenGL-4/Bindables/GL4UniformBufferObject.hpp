#pragma once

#include "Core/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	template<typename T>
	class GL4UniformBufferObject : public ShaderBufferObject<T>{
		//VARIABLES
	private:
		uint32 uboID = 0;

		uint32 bindingPoint = 0;

		//FUNCTIONS
	public:
		GL4UniformBufferObject() = delete;
		GL4UniformBufferObject(const GL4UniformBufferObject& other) = delete;
		GL4UniformBufferObject(GL4UniformBufferObject&& other) = default;
		GL4UniformBufferObject& operator=(const GL4UniformBufferObject& other) = delete;
		GL4UniformBufferObject& operator=(GL4UniformBufferObject&& other) = default;
		~GL4UniformBufferObject();

		GL4UniformBufferObject(uint32 bindingPoint);
		GL4UniformBufferObject(uint32 bindingPoint, T&& data);

		virtual void bind() override;

		virtual void update(T&& data) override;
	};
}

#include "GL4UniformBufferObject.inl"