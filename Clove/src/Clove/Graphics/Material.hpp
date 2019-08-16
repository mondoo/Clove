#pragma once

#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	class Shader;
	class Texture;

	class Material{
		//VARIABLES
	private:
		//Not quite sure how this will hold the shader. Need to look at perhaps refactoring how the vertex layout is done for dx to remove the dependancy 
		//std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		std::unordered_map<BufferBindingPoint, std::shared_ptr<Bindable>> shaderData;

		/*
		How would I handle multiple objects sharing a material, but having different positions?
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

		template<typename T>
		void setData(BufferBindingPoint bindingPoint, T&& data, gfx::ShaderType shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture); 

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

#include "Material.inl"