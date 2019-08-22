#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp" //TODO: Remove? needed for the shader reflection data

//TODO: Move this into the material file???
namespace clv::gfx{
	class Material;

	class MaterialInstance{
		//VARIABLES
	private:
		std::shared_ptr<Material> material;

		//FUNCTIONS
	public:
		MaterialInstance() = delete;
		MaterialInstance(const MaterialInstance& other) = default;
		MaterialInstance& operator=(const MaterialInstance& other) = default;
		MaterialInstance(MaterialInstance&& other) noexcept = default;
		MaterialInstance& operator=(MaterialInstance&& other) noexcept = default;

		MaterialInstance(std::shared_ptr<Material> material);

		void bind();

		const ShaderReflectionData& getReflectionData() const;
		const std::shared_ptr<Shader>& getShader() const;
	};
}