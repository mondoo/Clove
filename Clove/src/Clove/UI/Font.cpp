#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace clv::ui{
	std::weak_ptr<std::remove_pointer_t<FT_Library>> Font::ftLib = {};

	Font::Font(const std::string& filePath)
		: face(nullptr, nullptr)
		, filePath(filePath){

		if(ftLib.use_count() == 0){
			FT_Library library;
			if(FT_Init_FreeType(&library) != FT_Err_Ok){
				CLV_ASSERT(false, "Could not load freetype");
			} else{
				CLV_LOG_TRACE("Constructed FreeType library");
			}

			const auto libraryDeleter = [](FT_Library lib){
				CLV_LOG_TRACE("FreeType library has been deleted");
				FT_Done_FreeType(lib);
			};
			ftLibReference = std::shared_ptr<std::remove_pointer_t<FT_Library>>(library, libraryDeleter);
			ftLib = ftLibReference;
		} else{
			ftLibReference = ftLib.lock();
		}

		face = createFace(filePath);
	}

	Font::Font(const Font& other)
		: face(nullptr, nullptr){
		ftLibReference = other.ftLibReference;
		filePath = other.filePath;

		face = createFace(filePath);
	}

	Font::Font(Font&& other) noexcept = default;

	Font& Font::operator=(const clv::ui::Font& other){
		ftLibReference = other.ftLibReference;
		filePath = other.filePath;

		face = createFace(filePath);

		return *this;
	}

	Font& Font::operator=(Font&& other) noexcept = default;

	Font::~Font() = default;

	void Font::setSize(uint32 size){
		FT_Set_Pixel_Sizes(face.get(), 0, static_cast<FT_UInt>(size));
	}

	Glyph Font::getChar(char ch) const{
		FT_Load_Char(face.get(), ch, FT_LOAD_RENDER);
		return {
			math::Vector2f{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
			math::Vector2f{ face->glyph->bitmap_left, face->glyph->bitmap_top },
			math::Vector2f{ face->glyph->advance.x >> 6, face->glyph->advance.y >> 6 },
			face->glyph->bitmap.buffer
		};
	}

	std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> Font::createFace(const std::string& filePath) {
		FT_Face face;
		if(FT_New_Face(ftLibReference.get(), filePath.c_str(), 0, &face) != FT_Err_Ok){
			CLV_ASSERT(false, "Could not load font");
		} else{
			CLV_LOG_TRACE("Constructed FreeType font {0}", face->family_name);
		}

		const auto fontFaceDeleter = [](FT_Face face){
			CLV_LOG_TRACE("FreeType font face {0} has been deleted", face->family_name);
			FT_Done_Face(face);
		};
		return std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)>(face, fontFaceDeleter);
	}
}