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
	

small note to self
-I probably would just create a UI shader if I was using one API but I want as much usability as possible
-also I don't think I fully intended to implement the thing at the top it was more like I can do it if need be

-So the problem as I see it is that I want the gfx::Shader class to handle the creation of shaders.
But right now it is also responsible for setting some buffers / uniforms (opengl and DX also do this differently, but it won't be too much effort to change that)
	-I think this is why I wanted the shader buffer thing. But the problem is not how the data is represented (it's definitley fine for the time being)
	But more so how many datas I will need to give to the shader.
-Should these shader classes reflect the type of shaders put into the pipeline? seems like it could get real messy



-I think the best thing to do would be to remove the buffer data passing out from the shader and have that handled externaly (like moving uniforms inside textures or setting upo materials etc.)



-I suppose I couold make another type that you can add to a shader that'll get bound when the shader gets bound.
	-So ShaderBufferObjects will be the 'global' one (like how it's used now, might need a name change)
	-Then we'll have a shader specific one (like the GL4Uniform class but the DX one will just be a wrapper for a cb)
		-These can be initialised by like a mesh component or something and then given to a shader and the shader will binding those objects in when it gets bound so the data will stay syncronised

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