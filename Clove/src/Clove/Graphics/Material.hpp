#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp" //TODO: remove? needed for the reflection data

namespace clv::gfx{
	class Shader;
	class Texture;
	class MaterialInstance;

	class Material : public std::enable_shared_from_this<Material>{
		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		//std::unordered_map<BufferBindingPoint, std::shared_ptr<Bindable>> shaderData;

		ShaderReflectionData reflectionData;

		//FUNCTIONS
	public:
		Material() = delete;
		Material(const Material& other);
		Material& operator=(const Material& other);
		Material(Material&& other) noexcept;
		Material& operator=(Material&& other) noexcept;
		~Material();

		Material(ShaderStyle shaderStyle);

		void bind();

		MaterialInstance createInstance(); //wish it could be const :(

		//TODO: Maybe not have these getters?
		const ShaderReflectionData& getReflectionData() const;
		const std::shared_ptr<Shader>& getShader() const;

		/*
		how do i want the interface to look?

		I'd want to only make SBOs for the buffers a shader has (not like below where it'll just make one)
		The instances will cache of data for these buffers and set them when binding

		If i can get the binding point from the CBs then I just need to figure out what types
		the buffers will take to construct the sbos
			--types are possible with DX
			--binding points are possible with OpenGL

		then I just need to figure out how to check for all of this

		honestly though the SBOs only need types for their benefit - ut does just get sent as raw data
			--Can get size of CB with DX
			--Can get size of UBO with GL

		how will I identify what buffers to set? using the name?
			--I think the name is the best way, then I can use the data avilable to check for types etc.
		*/


		template<typename T>
		void setData(BufferBindingPoint bindingPoint, T&& data, gfx::ShaderType shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture); 

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

#include "Material.inl"