#pragma once

#include "Bulb/Rendering/Material.hpp"

namespace clv::gfx{
	class CommandBuffer;
	class Texture;
}

namespace blb::rnd{
	class Material;
}

namespace blb::rnd{
	class MaterialInstance{
		//VARIABLES
	private:
		std::shared_ptr<Material> material;
		std::shared_ptr<clv::gfx::Texture> albedoTexture;
		std::shared_ptr<clv::gfx::Texture> specTexture;
		std::unordered_map<clv::gfx::BufferBindingPoint, ShaderData> shaderData;

		//FUNCTIONS
	public:
		MaterialInstance() = delete;
		MaterialInstance(std::shared_ptr<Material> material);

		MaterialInstance(const MaterialInstance& other);
		MaterialInstance(MaterialInstance&& other) noexcept;

		MaterialInstance& operator=(const MaterialInstance& other);
		MaterialInstance& operator=(MaterialInstance&& other) noexcept;

		~MaterialInstance();

		void bind(clv::gfx::CommandBuffer& commandBuffer);

		template<typename DataType>
        void setData(clv::gfx::BufferBindingPoint bindingPoint, DataType&& data, clv::gfx::ShaderStage shaderType);
        void setData(clv::gfx::BufferBindingPoint bindingPoint, void* data, const size_t sizeBytes, clv::gfx::ShaderStage shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(std::shared_ptr<clv::gfx::Texture> texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(std::shared_ptr<clv::gfx::Texture> texture);
	};
}

#include "MaterialInstance.inl"