#include "clvpch.hpp"
#include "MaterialInstance.hpp"

#include "Clove/Graphics/Material.hpp"

namespace clv::gfx{
	MaterialInstance::MaterialInstance(std::shared_ptr<Material> material)
		: material(std::move(material)){
	}

	void MaterialInstance::bind(){
		material->bind();
	}
	
	const ShaderReflectionData& MaterialInstance::getReflectionData() const{
		return material->getReflectionData();
	}
	
	const std::shared_ptr<Shader>& MaterialInstance::getShader() const{
		return material->getShader();
	}
}