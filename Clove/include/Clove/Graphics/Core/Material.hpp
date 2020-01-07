#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"
#include "Clove/Graphics/Core/Resources/Buffer.hpp"

namespace clv::gfx{
	class MaterialInstance;

	struct ShaderData{
		std::shared_ptr<Buffer> buffer;
		ShaderType shaderType;
	};
}

namespace clv::gfx{
	class Material : public std::enable_shared_from_this<Material>{
		friend class MaterialInstance;

		//VARIABLES
	private:
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		std::unordered_map<BufferBindingPoint, ShaderData> shaderData;

		//FUNCTIONS
	public:
		Material();

		Material(const Material& other);
		Material(Material&& other) noexcept;

		Material& operator=(const Material& other);
		Material& operator=(Material&& other) noexcept;

		~Material();

		MaterialInstance createInstance();

		template<typename DataType>
		void setData(BufferBindingPoint bindingPoint, DataType&& data, ShaderType shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

#include "Material.inl"