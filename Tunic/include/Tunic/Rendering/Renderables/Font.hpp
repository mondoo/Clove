#pragma once

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

namespace clv::gfx{
	class GraphicsFactory;
	class Texture;
}

namespace tnc::rnd{
	struct Glyph{
		clv::mth::vec2f size{};
		clv::mth::vec2f bearing{};
		clv::mth::vec2f advance{};
		std::shared_ptr<clv::gfx::Texture> character;
	};
}

namespace tnc::rnd{
	class Font{
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

		static std::weak_ptr<std::remove_pointer_t<FT_Library>> ftLib;
		
		std::shared_ptr<std::remove_pointer_t<FT_Library>> ftLibReference;
		std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> face;

		std::string filePath;

		//FUNCTIONS
	public:
		Font() = delete;
		Font(const std::string& filePath, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);

		Font(const Font& other);
		Font(Font&& other) noexcept;

		Font& operator=(const Font& other);
		Font& operator=(Font&& other) noexcept;

		~Font();
		
		void setFontSize(uint32_t size);
		Glyph getChar(char ch) const;

	private:
		std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> createFace(const std::string& filePath);
	};
}