#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	class GLTexture : public Texture{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		std::string filePath;

		int width = 0;
		int height = 0;
		int BPP = 0;

		//FUNCTIONS
	public:
		GLTexture() = delete;
		GLTexture(const GLTexture& other) = delete;
		GLTexture(GLTexture&& other) noexcept;
		GLTexture& operator=(const GLTexture& other) = delete;
		GLTexture& operator=(GLTexture&& other) noexcept;
		virtual ~GLTexture();

		GLTexture(const std::string& filePath);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;
	};
}
