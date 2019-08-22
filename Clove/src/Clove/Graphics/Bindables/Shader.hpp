#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/VertexLayout.hpp" //TODO: Remove? to get the input types

namespace clv::gfx{
	struct ShaderBufferDescription{
		std::string bufferName;
		//How will I represent what types the buffer has inside it?
		//--CBs will have a list of variables inside the cb
		//--Looks like it might be possible with UBOS? can get the number / indicies of the uniforms in the buffer
	};

	struct ShaderReflectionData{
		VertexLayout vertexBufferLayout;
		std::vector<ShaderBufferDescription> bufferDescriptions;
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