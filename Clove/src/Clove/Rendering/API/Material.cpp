#include "clvpch.h"
#include "Material.h"

#include "Clove/Rendering/API/Shader.h"
#include "Clove/Rendering/API/Texture.h"

namespace clv{
	Material::Material(){
		createMaterial("../Clove/res/Textures/DefaultTexture.png");
	}

	Material::Material(const std::string& texturePath){
		createMaterial(texturePath);
	}

	Material::Material(const Material& other){
		CLV_WARN("Copy constructor called on material - creating new material data");
		createMaterial(other.texturePath);
	}

	Material::Material(Material&& other){
		shader = std::move(other.shader);
		texture = std::move(other.texture);
		texturePath = other.texturePath;
	}

	Material::~Material() = default;

	void Material::bindShader(){
		shader->bind();
	}

	void Material::unbindShader(){
		shader->unbind();
	}

	void Material::bindTexture(){
		const unsigned int slot = 0;
		texture->bind(slot);
		shader->bind();
		shader->setUniform<int>("textureSample", slot);
	}

	void Material::unbindTexture(){
		texture->unbind();
		shader->unbind();
	}

	void clv::Material::setUniform1i(const std::string& name, int value){
		shader->bind();
		shader->setUniform(name, value);
	}

	void Material::setUniform1f(const std::string& name, float value){
		shader->bind();
		shader->setUniform(name, value);
	}

	void Material::setUniform3f(const std::string& name, const glm::vec3& value){
		shader->bind();
		shader->setUniform(name, value);
	}

	void Material::setUniform4f(const std::string& name, const glm::vec4& value){
		shader->bind();
		shader->setUniform(name, value);
	}

	void Material::setUniform4m(const std::string& name, const glm::mat4& value){
		shader->bind();
		shader->setUniform(name, value);
	}

	Material& Material::operator=(const Material& other){
		CLV_WARN("Copy assignment operator called on material - creating new material data");
		createMaterial(other.texturePath);

		return *this;
	}

	Material& Material::operator=(Material&& other){
		shader = std::move(other.shader);
		texture = std::move(other.texture);
		texturePath = other.texturePath;

		return *this;
	}

	void Material::createMaterial(const std::string& texturePath){
		this->texturePath = texturePath;
		
		shader = std::make_unique<Shader>(Shader());

		shader->attachShader(ShaderTypes::Vertex, "../Clove/res/Shaders/VertexShader.glsl");
		shader->attachShader(ShaderTypes::Fragment, "../Clove/res/Shaders/FragmentShader.glsl");

		texture = std::make_unique<Texture>(Texture(texturePath));
	}
}