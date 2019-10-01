#pragma once

// has the font thing

/*
Inteprets font files into some generic data that can be used to create tex coords ect.

TODO: put inside utility???
*/


/*
-a reusable class
-holds the data of the font, can be used to extract each character

*/

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

namespace clv::ui{
	struct Glyph{
		math::Vector2f size{};
		math::Vector2f bearing{};
		math::Vector2f advance{};
		uint8* buffer = nullptr;
	};

	class Font{ //I wonder if we need like a 2d drawable interface?? (although, what would that solve?)
		//VARIABLES
	private:
		FT_Library ft = nullptr;
		FT_Face face = nullptr;

		//FUNCTIONS
	public:
		//TODO: others
		Font() = delete;
		//Font(const Font& other) = delete; //TODO: Should copy make a new face?
		//Font& operator=(const Font& other) = delete;
		//Font(Font&& other) noexcept;
		//Font& operator=(Font&& other) noexcept;
		~Font();

		Font(const std::string& filePath);
		
		void setSize(uint32 size);
		Glyph getChar(char ch) const;
	};
}