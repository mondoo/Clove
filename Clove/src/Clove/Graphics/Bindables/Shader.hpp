#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
	class Renderer;

	enum class ShaderTypes{
		Vertex,
		Pixel,
	};

	class Shader : public Bindable{
		//FUNCTIONS
	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		virtual ~Shader();

		virtual void attachShader(ShaderTypes type, Renderer& renderer) = 0;

		virtual void setWorldMatrix(const math::Matrix4f& world) = 0;
		virtual void setViewMatrix(const math::Matrix4f& view) = 0;
		virtual void setProjectionMatrix(const math::Matrix4f& projection) = 0;
	};
}