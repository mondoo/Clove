#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"
#include "Clove/Graphics/Core/Resources/Buffer.hpp"

namespace clv::gfx{
	struct ShaderData{
		std::shared_ptr<Buffer> buffer;
		ShaderType shaderType;
	};
}

namespace tnc::rnd{
	class MaterialInstance;
}

namespace tnc::rnd{
	class Material : public std::enable_shared_from_this<Material>{
		friend class MaterialInstance;

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Texture> albedoTexture;
		std::shared_ptr<clv::gfx::Texture> specTexture;
		std::unordered_map<clv::gfx::BufferBindingPoint, clv::gfx::ShaderData> shaderData;

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
		void setData(clv::gfx::BufferBindingPoint bindingPoint, DataType&& data, clv::gfx::ShaderType shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<clv::gfx::Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<clv::gfx::Texture>& texture);
	};
}

#include "Material.inl"