#pragma once

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

namespace clv::ui{
	struct Glyph{
		mth::vec2f size = {};
		mth::vec2f bearing = {};
		mth::vec2f advance = {};
		uint8* buffer = nullptr;
	};

	class Font{
		//VARIABLES
	private:
		static std::weak_ptr<std::remove_pointer_t<FT_Library>> ftLib;
		
		std::shared_ptr<std::remove_pointer_t<FT_Library>> ftLibReference;
		std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> face;

		std::string filePath;

		//FUNCTIONS
	public:
		Font() = delete;
		Font(const Font& other);
		Font(Font&& other) noexcept;
		Font& operator=(const Font& other);
		Font& operator=(Font&& other) noexcept;
		~Font();

		Font(const std::string& filePath);
		
		void setSize(uint32 size);
		Glyph getChar(char ch) const;

	private:
		std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> createFace(const std::string& filePath);
	};
}