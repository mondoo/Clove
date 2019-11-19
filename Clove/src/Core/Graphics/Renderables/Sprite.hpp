#pragma once

namespace clv::gfx{
	class Texture;
}

namespace clv::gfx{
	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Texture> texture;
		math::Vector4f colour{ 1.0f, 1.0f, 1.0f, 1.0f };
		math::Matrix4f modelData{};

		//FUNCTIONS
	public:
		Sprite();
		Sprite(const std::string& pathToTexture);
		Sprite(std::shared_ptr<Texture> texture);

		Sprite(const Sprite& other);
		Sprite(Sprite&& other) noexcept;

		Sprite& operator=(const Sprite& other);
		Sprite& operator=(Sprite&& other) noexcept;

		~Sprite();

		const std::shared_ptr<Texture>& getTexture() const;

		void setColour(const math::Vector4f& colour);
		const math::Vector4f& getColour() const;

		void setModelData(const math::Matrix4f& modelData);
		const math::Matrix4f& getModelData() const;
	};
}
