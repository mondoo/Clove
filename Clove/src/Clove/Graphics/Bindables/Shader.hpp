#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class Shader : public Bindable{
		//FUNCTIONS
	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		virtual ~Shader();

		//Something here to set a value with a binding point?
		//or should I keep it as a seperate entity?
		//Perhaps this will be handled in the material?
	};
}