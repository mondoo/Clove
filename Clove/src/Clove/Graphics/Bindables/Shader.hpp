#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

/*
-constant buffers and uniform buffer objects take a pointer to some data and a binding point
	-uniforms kind of do but don't
-Looks like there may be a way to achieve this by doing something similar to how we did the vertex data
	-how some sort of shader data
	-to be honest it could be way more simple and char take a char* because the layout of the bytes is a lot less important
		-how will this fair with uniforms? will we still use them? 
		-we do need the size of the entire data set though and alignment is important

-Type abstraction can be done in the buffer thing and all the cb/ub stuff can just grab the pointer and size

-What was this trying to solve?

-Shader holds a bunch of cb/u/ubs
	-I want to get rid of that and just have the shader manage the api calls for creating shaders
		-but... ?
		-Well I guess it's because I can define those buffers outside of the shader and bind them in
			-But why did I need the abstraction?

-The problem was that the current shader is coupled to the actual shaders and their data. 
 However, Could I just not move these out of the shader?

-It was to give to the buffers inside the shader so I wouldn't need to pull them out. Maybe I should though? Like make a material?
	
*/

namespace clv::gfx{
	class Renderer;

	class Shader : public Bindable{
		//FUNCTIONS
	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		virtual ~Shader();

		virtual void attachShader(ShaderTypes type) = 0;

		virtual void setModelMatrix(const math::Matrix4f& world) = 0;
	};
}