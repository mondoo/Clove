#pragma once

namespace clv::gfx{
	class Texture;
}

namespace tnc::rnd{
	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Texture> texture;
		clv::mth::vec4f colour{ 1.0f, 1.0f, 1.0f, 1.0f };
		clv::mth::mat4f modelData{};

		//FUNCTIONS
	public:
		Sprite();
		Sprite(const std::string& pathToTexture);
		Sprite(std::shared_ptr<clv::gfx::Texture> texture);

		Sprite(const Sprite& other);
		Sprite(Sprite&& other) noexcept;

		Sprite& operator=(const Sprite& other);
		Sprite& operator=(Sprite&& other) noexcept;

		~Sprite();

		const std::shared_ptr<clv::gfx::Texture>& getTexture() const;

		void setColour(const clv::mth::vec4f& colour);
		const clv::mth::vec4f& getColour() const;

		void setModelData(const clv::mth::mat4f& modelData);
		const clv::mth::mat4f& getModelData() const;
	};
}
