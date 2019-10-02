#include "clvpch.hpp"
#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace clv::ui{
	Font::Font(const std::string& filePath)
		: ft(nullptr, nullptr)
		, face(nullptr, nullptr){

		FT_Library library;
		FT_Face face;

		//TODO: This should only happen once
		if(FT_Init_FreeType(&library)){
			CLV_ASSERT(false, "Could not load freetype");
		} else{
			CLV_LOG_TRACE("Constructed FreeType library");
		}
		//

		if(FT_New_Face(library, filePath.c_str(), 0, &face)){
			CLV_ASSERT(false, "Could not load font");
		} else{
			CLV_LOG_TRACE("Constructed FreeType font {0}", face->family_name);
		}

		ft = std::unique_ptr<std::remove_pointer_t<FT_Library>, void(*)(FT_Library)>(library, &Font::freeFontLibrary);
		this->face = std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)>(face, &Font::freeFontFace);
	}

	Font::Font(Font&& other) noexcept = default;

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

	void Font::freeFontLibrary(FT_Library lib){
		CLV_LOG_TRACE("FreeType library has been deleted");
		FT_Done_FreeType(lib);
	}

	void Font::freeFontFace(FT_Face face){
		CLV_LOG_TRACE("FreeType font face {0} has been deleted", face->family_name);
		FT_Done_Face(face);
	}
}