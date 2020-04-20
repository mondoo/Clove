#pragma once

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;

namespace clv::gfx {
	class GraphicsFactory;
	class Texture;
}

namespace blb::rnd {
	struct Glyph {
		clv::mth::vec2f size{};
		clv::mth::vec2f bearing{};
		clv::mth::vec2f advance{};
		std::shared_ptr<clv::gfx::Texture> character;
	};
}

namespace blb::rnd {
	class Font {
		//TYPES
	public:
		using FTLibWeakPtr		= std::weak_ptr<std::remove_pointer_t<FT_Library>>;
		using FTLibSharedPtr	= std::shared_ptr<std::remove_pointer_t<FT_Library>>;

		using FacePtr			= std::unique_ptr<std::remove_pointer_t<FT_Face>, void (*)(FT_Face)>;

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

		static FTLibWeakPtr ftLib;
		FTLibSharedPtr		ftLibReference;

		FacePtr face;

		//FUNCTIONS
	public:
		Font() = delete;
		Font(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		Font(const std::string& filePath, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		Font(const unsigned char* bytes, const std::size_t size, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);

		Font(const Font& other);
		Font(Font&& other) noexcept;

		Font& operator=(const Font& other);
		Font& operator=(Font&& other) noexcept;

		~Font();

		void setFontSize(uint32_t size);
		Glyph getChar(char ch) const;

	private:
		FacePtr createFace(const std::string& filePath);
		FacePtr createFace(const unsigned char* bytes, const std::size_t size);
	};
}