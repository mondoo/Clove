#pragma once

namespace clv{
	class CLV_API Texture{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		std::string filePath;
		unsigned char* localBuffer = nullptr; //needed as a member variable?
		int width = 0;
		int height = 0;
		int BPP = 0;

		//FUNCTIONS
	public:
		Texture(const std::string& filePath);
		Texture(Texture&& other);

		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		inline int getWidth() const{ return width; }
		inline int getHeight() const{ return height; }

		Texture& operator=(Texture&& other);
	};
}