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

	/*
	make sbo buffer size of the ub / cb, then it can divide that into sections or what ever depending
	on the size of each individual param
	*/

	//TODO: Move all of this into another class
	enum class BufferVariableType{
		int1,
		float1,
		float2,
		float3,
		float4,
		float4_4
	};
	
	template<BufferVariableType> struct BufferVariable;
	template<> struct BufferVariable<BufferVariableType::int1>{
		//Something like this?
	};

	struct ShaderBufferVariable{
		std::string name;
		BufferVariableType type;
		size_t size; //in gl this'll always be 1 unless the variable is a struct or array
					//worst come I'll have to figure out the size from the types
					//still not sure how to handle the arrays though
	};

	struct ShaderBufferDescription{
		std::string name;
		uint32 bindingPoint;
		size_t totalSize;
		std::vector<ShaderBufferVariable> variables;
	};
	//~TODO

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