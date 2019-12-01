#pragma once

namespace clv::gfx{
	class Texture;
}

namespace clv::gfx{
	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Texture> texture;
		mth::vec4f colour{ 1.0f, 1.0f, 1.0f, 1.0f };
		mth::mat4f modelData{};

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

		void setColour(const mth::vec4f& colour);
		const mth::vec4f& getColour() const;

		void setModelData(const mth::mat4f& modelData);
		const mth::mat4f& getModelData() const;
	};
}
