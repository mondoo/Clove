#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class Renderer;

	enum class ShaderValueType{
		VS_View,

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

		virtual void attachShader(ShaderType type) = 0;

		//I am thinking something like this for the shaders
		//virtual void setValueOnShader(/*some enum type for what it is (model data etc.)*/);
	};
}