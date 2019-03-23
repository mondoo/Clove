#pragma once

#include "Clove/Rendering/Renderer.h"

namespace clv{
	class CLV_API Texture{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		std::string filePath;
		unsigned char* localBuffer = nullptr;
		int width = 0;
		int height = 0;
		int BPP = 0;

		//FUNCTIONS
	public:
		Texture() = default; //temp
		Texture(const std::string& filePath);
		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		void deleteTexture();

		inline int getWidth() const{ return width; }
		inline int getHeight() const{ return height; }
	};
}