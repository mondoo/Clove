#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	struct ShaderReflectionData{
		//IN PROGRESS - what other info do I need?

		//BufferBindingPoint bindingPoint;
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

		//TODO: This will have to be handled in each shader class
		//Or maybe they'll call off to some helper functions
		virtual std::vector<ShaderReflectionData> getReflectionData() = 0;
	};
}