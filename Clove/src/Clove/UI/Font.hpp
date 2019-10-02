#pragma once

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

	class Font{
		//VARIABLES
	private:
		std::unique_ptr<std::remove_pointer_t<FT_Library>, void(*)(FT_Library)> ft;
		std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> face;

		//FUNCTIONS
	public:
		Font() = delete;
		Font(const Font& other) = delete;
		Font& operator=(const Font& other) = delete;
		Font(Font&& other) noexcept;
		Font& operator=(Font&& other) noexcept;
		~Font();

		Font(const std::string& filePath);
		
		void setSize(uint32 size);
		Glyph getChar(char ch) const;

	private:
		static void freeFontLibrary(FT_Library lib);
		static void freeFontFace(FT_Face face);
	};
}