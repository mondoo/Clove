#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/VertexLayout.hpp" //TODO: Remove? to get the input types

namespace clv::gfx{
	struct ShaderReflectionData{
		//std::vector<std::string> inputArgumentTypes;
		//std::vector<int32> sboBindingPoints; //TODO: Do I need to do this or is the current system suffcient
		VertexLayout vertexBufferLayout;
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

		virtual ShaderReflectionData getReflectionData() = 0;
	};
}