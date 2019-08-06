#pragma once

namespace clv::gfx{
	class Shader;
	class Texture;

	class Material{
		//VARIABLES
	private:
		math::Matrix4f modelData{}; //This should be an sbo

		//Not quite sure how this will hold the shader. Need to look at perhaps refactoring how the vertex layout is done for dx to remove the dependancy 
		//std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;

		/*
		holds data about a mesh looks (and maybe sprites?)
		-holds the textures
		-holds the sbos (or the BufferData class/struct that will replace)
		-I think it should hold the shader but that might be difficult
		*/

		/*
		How do I want to handle the data coming in?
		-Was thinking of abstracting away behind some class. 
		--it gives us a char array and a binding point
		--gets converted to a ubo or cb somewhere down the line

		-How do i know what type of data to give it?
		--Some will get handled by the render system
		--some I'll just have to do myself for now
		*/

		//FUNCTIONS
	public:
		Material();
		Material(const Material& other);
		Material& operator=(const Material& other);
		Material(Material&& other) noexcept;
		Material& operator=(Material&& other) noexcept;
		~Material();

		void bind(); //this'll bind the shader and textures

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

