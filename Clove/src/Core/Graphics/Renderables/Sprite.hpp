#pragma once

namespace clv::gfx{
	class Texture;
}

namespace clv::gfx{
	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Texture> texture;
		math::Matrix4f modelData{};

		//FUNCTIONS
	public:
		Sprite() = delete;
		Sprite(const std::string& pathToTexture);
		Sprite(std::shared_ptr<Texture> texture);

		Sprite(const Sprite& other);
		Sprite(Sprite&& other) noexcept;

		Sprite& operator=(const Sprite& other);
		Sprite& operator=(Sprite&& other) noexcept;

		~Sprite();

		const std::shared_ptr<Texture>& getTexture() const;

		void setModelData(const math::Matrix4f& modelData);
		const math::Matrix4f& getModelData() const;
	};
}
