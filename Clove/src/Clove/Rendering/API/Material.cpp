#include "clvpch.hpp"
#include "Material.hpp"

#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/API/Texture.hpp"

namespace clv{
	Material::Material(){
		createMaterial("../Clove/res/Textures/DefaultTexture.png");
	}

	Material::Material(const std::string& texturePath){
		createMaterial(texturePath);
	}

	Material::Material(const Material& other){
		CLV_WARN("Copy constructor called on material - creating new material data");
		
		createMaterial(other.diffuseTexturePath);
		
		if(!other.specularTexturePath.empty()){
			createSpecularTexture(other.specularTexturePath);
		} else{
			specularTexturePath.clear();
			specularTexture.reset();
		}
	}

	Material::Material(Material&& other) noexcept{
		diffuseTexture = std::move(other.diffuseTexture);
		specularTexture = std::move(other.specularTexture);

		diffuseTexturePath = std::move(other.diffuseTexturePath);
		specularTexturePath = std::move(other.specularTexturePath);

		uniformMap = std::move(other.uniformMap);
	}

	Material::~Material() = default;

	void Material::setSpecularTexture(const std::string& path){
		createSpecularTexture(path);
	}

	void Material::bind(Shader& shader){
		diffuseTexture->bind(1);
		shader.setUniform<int>("material.diffuse", 1);
		if(specularTexture){
			specularTexture->bind(2);
			shader.setUniform<int>("material.specular", 2);
		}

		for(const auto& [key, value] : uniformMap){
			if(value.type() == typeid(int)){
				shader.setUniform<int>(key, std::any_cast<int>(value));
			} else if(value.type() == typeid(float)){
				shader.setUniform<float>(key, std::any_cast<float>(value));
			} else if(value.type() == typeid(math::Vector3f)){
				shader.setUniform<math::Vector3f>(key, std::any_cast<math::Vector3f>(value));
			} else if(value.type() == typeid(math::Vector4f)){
				shader.setUniform<math::Vector4f>(key, std::any_cast<math::Vector4f>(value));
			} else if(value.type() == typeid(math::Matrix4f)){
				shader.setUniform<math::Matrix4f>(key, std::any_cast<math::Matrix4f>(value));
			} else{
				shader.setUniform(key, value);
			}
		}
	}

	void Material::unbind(){
		diffuseTexture->unbind();
		if(specularTexture){
			specularTexture->unbind();
		}
	}

	Material& Material::operator=(const Material& other){
		CLV_WARN("Copy assignment operator called on material - creating new material data");
		
		createMaterial(other.diffuseTexturePath);
		
		if(!other.specularTexturePath.empty()){
			createSpecularTexture(other.specularTexturePath);
		} else{
			specularTexturePath.clear();
			specularTexture.reset();
		}

		return *this;
	}

	Material& Material::operator=(Material&& other) noexcept = default;

	void Material::createMaterial(const std::string& texturePath){
		CLV_TRACE("Creating material with: {0}", texturePath);

		diffuseTexturePath = texturePath;
		diffuseTexture = std::make_unique<Texture>(Texture(texturePath));
	}

	void Material::createSpecularTexture(const std::string& texturePath){
		specularTexturePath = texturePath;
		specularTexture = std::make_unique<Texture>(Texture(texturePath));
	}
}