#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"

namespace clv::gfx{
	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Texture> texture;
		std::shared_ptr<ShaderBufferObject<math::Matrix4f>> modelData;

		//FUNCTIONS
	public:
		Sprite() = delete;
		Sprite(const Sprite& other);
		Sprite& operator=(const Sprite& other);
		Sprite(Sprite&& other) noexcept;
		Sprite& operator=(Sprite&& other) noexcept;
		~Sprite();

		Sprite(const std::string& pathToTexture);
		Sprite(std::shared_ptr<Texture> texture);

		void bind();

		void setModelData(const math::Matrix4f& modelData);

	private:
		void initialise();
	};
}
