#include "clvpch.hpp"
#include "Material.hpp"

#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/MaterialInstance.hpp"

namespace clv::gfx{
	Material::Material() = default;

	Material::Material(const Material& other) = default;

	Material& Material::operator=(const Material& other) = default;

	Material::Material(Material&& other) noexcept{
		albedoTexture = std::move(other.albedoTexture);
		specTexture = std::move(other.specTexture);
		shaderData = std::move(other.shaderData);
	}

	Material& Material::operator=(Material&& other) noexcept = default;

	Material::~Material() = default;

	void Material::bind(){
		//shader->bind();
		albedoTexture->bind();
		if(specTexture){
			specTexture->bind();
		}
		for(auto& [key, val] : shaderData){
			val->bind();
		}
	}

	void Material::linkShader(const std::shared_ptr<Shader>& shader){
		/*
		TODO (also isn't called yet):
		https://docs.microsoft.com/en-us/windows/win32/api/d3d11shader/nn-d3d11shader-id3d11shaderreflection
		https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
		https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniformBlock.xhtml
		
		
		*/

		for(ShaderReflectionData& data : shader->getReflectionData()){
			//This data will need either need to be used as is or converted into some form of material data
			//Will these just be used to make the UBOs???
		}
	}

	std::unique_ptr<MaterialInstance> Material::createInstance() const{
		//TODO:
		/*
		probably give the material instance the binding points etc.
		*/
		return {};
	}

	void Material::setAlbedoTexture(const std::string& path){
		albedoTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}

	void Material::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Albedo, "Incorrect binding point for an albedo texture!");
		albedoTexture = texture;
	}

	void Material::setSpecularTexture(const std::string& path){
		specTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Specular);
	}

	void Material::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Specular, "Incorrect binding point for a specular texture!");
		specTexture = texture;
	}
}