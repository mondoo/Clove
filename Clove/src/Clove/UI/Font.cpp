#include "clvpch.hpp"
#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace clv::ui{
	Font::Font(const std::string& filePath){
		//TODO: This should only happen once
		if(FT_Init_FreeType(&ft)){
			CLV_ASSERT(false, "Could not load freetype");
		}

		if(FT_New_Face(ft, filePath.c_str(), 0, &face)){
			CLV_ASSERT(false, "Could not load font");
		}

		//FT_Set_Pixel_Sizes(face, 0, 78); //This could be done on a text box or something

	}

	//Font::Font(const Font& other){
	//}

	//Font& Font::operator=(const Font& other){
	//	// TODO: insert return statement here
	//}

	/*Font::Font(Font&& other) noexcept{
		ft = std::move(other.ft);
		face = std::move(other.face);
	}

	Font& Font::operator=(Font&& other) noexcept{
		ft = std::move(other.ft);
		face = std::move(other.face);

		return *this;
	}*/

	Font::~Font(){
		//TODO: LEAKING HERE - there is a problem where this gets deconstructed and thus frees resources out of copied fonts

		//FT_Done_Face(face);
		//TODO: This should only happen once
		//FT_Done_FreeType(ft);
	}

	void Font::setSize(uint32 size){
		FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(size));
	}

	Glyph Font::getChar(char ch) const{
		FT_Load_Char(face, ch, FT_LOAD_RENDER);
		return {
			math::Vector2f{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
			math::Vector2f{ face->glyph->bitmap_left, face->glyph->bitmap_top },
			math::Vector2f{ face->glyph->advance.x >> 6, face->glyph->advance.y >> 6 },
			face->glyph->bitmap.buffer
		};
	}
}