#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Texture.hpp"
#include "Clove/Graphics/Buffer.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace tnc::rnd{
	class MaterialInstance;

	struct ShaderData {
		std::shared_ptr<clv::gfx::Buffer> buffer;
		clv::gfx::ShaderStage shaderType;
	};
}

namespace tnc::rnd{
	class Material : public std::enable_shared_from_this<Material>{
		friend class MaterialInstance;

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

		std::shared_ptr<clv::gfx::Texture> albedoTexture;
		std::shared_ptr<clv::gfx::Texture> specTexture;
		std::unordered_map<clv::gfx::BufferBindingPoint, ShaderData> shaderData;

		//FUNCTIONS
	public:
		Material(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);

		Material(const Material& other);
		Material(Material&& other) noexcept;

		Material& operator=(const Material& other);
		Material& operator=(Material&& other) noexcept;

		~Material();

		MaterialInstance createInstance();

		template<typename DataType>
		void setData(clv::gfx::BufferBindingPoint bindingPoint, DataType&& data, clv::gfx::ShaderStage shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(std::shared_ptr<clv::gfx::Texture> texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(std::shared_ptr<clv::gfx::Texture> texture);
	};
}

#include "Material.inl"