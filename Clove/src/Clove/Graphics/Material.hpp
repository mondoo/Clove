#pragma once

//model data
//shader to set paramters on (if any, or at least to bind when we want to use this material?)
//could even hold the sbo for the model data

namespace clv::gfx{
	class Shader;
	class Texture;

	class Material{
		//VARIABLES
	private:
		math::Matrix4f modelData{}; //This should be an sbo

		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;

		//FUNCTIONS
	public:
		Material();
		Material(const Material& other);
		Material& operator=(const Material& other);
		Material(Material&& other) noexcept;
		Material& operator=(Material&& other) noexcept;
		~Material();

		void bind(); //this'll bind the shader and textures
	};
}

