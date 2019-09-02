#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/VertexLayout.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp" //TODO: Remove? to get the BufferBindingPoint

namespace clv::gfx{
	//How to construct an SBO from this?
	/*
	in the case of the array it'd be easier to know it's an array
	also outside objects (like the renderer) are going to need to a struct or something like they do now
	I think this buffer variable/description is going to need to be a huge class like the vertex data
	with a way to set types and then just a pointer to where to put the data? then the SBO can just bind that in
	
	We can get:
		GL:
		-UBname
		-uniforms in UB
			-name (for array objects it's literally lights[0].linear)
			-type
			-size

		DX:
		-CBname
		-variables in CB
			-name
			-type
			-size
	

	Okay so:
	-ShaderBufferDescription will need a pointer to the a memory block that all of the variables get put in
	-the SBOs can just bind this (they don't need to know the type of data, we'll do that another way)
	-it might be easier to have GL and DX version (we'll see though)
	*/

	struct ShaderBufferVariable{

	};

	struct ShaderBufferDescription{
		std::string bufferName;
		BufferBindingPoint bindingPoint;
		std::vector<ShaderBufferVariable> bufferVariables;

		//How will I represent what types the buffer has inside it?
		//--CBs will have a list of variables inside the cb
		//--Looks like it might be possible with UBOS? can get the number / indicies of the uniforms in the buffer
	};

	struct ShaderReflectionData{
		VertexLayout vertexBufferLayout;
		//Will we need one for samplers / texture slots??
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