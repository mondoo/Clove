#include "Tunic/Rendering/Renderables/Font.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/Texture.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace clv;
using namespace clv::gfx;

extern "C" const unsigned char roboto_black[];
extern "C" const size_t roboto_blackLength;

namespace tnc::rnd {
	static Font::FacePtr makeUniqueFace(FT_Face face) {
		return Font::FacePtr(face, [](FT_Face face) { FT_Done_Face(face); });
	}

	static Font::FacePtr copyFace(FT_Face face) {
		if(FT_Reference_Face(face) != FT_Err_Ok) {
			CLV_ASSERT(false, "Could not reference face");
		}
		return makeUniqueFace(face);
	}

	Font::FTLibWeakPtr Font::ftLib = {};

	Font::Font(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
		: graphicsFactory(std::move(graphicsFactory))
		, face(nullptr, nullptr) {

		if(ftLib.use_count() == 0) {
			FT_Library library;
			if(FT_Init_FreeType(&library) != FT_Err_Ok) {
				CLV_ASSERT(false, "Could not load freetype");
			} else {
				CLV_LOG_TRACE("Constructed FreeType library");
			}

			const auto libraryDeleter = [](FT_Library lib) {
				CLV_LOG_TRACE("FreeType library has been deleted");
				FT_Done_FreeType(lib);
			};
			ftLibReference = FTLibSharedPtr(library, libraryDeleter);
			ftLib = ftLibReference;
		} else {
			ftLibReference = ftLib.lock();
		}

		face = createFace(roboto_black, roboto_blackLength);
	}

	Font::Font(const std::string& filePath, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
		: Font(std::move(graphicsFactory)) {
		face = createFace(filePath);
	}

	Font::Font(const unsigned char* bytes, const std::size_t size, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
		: Font(std::move(graphicsFactory)) {
		face = createFace(bytes, size);
	}

	Font::Font(const Font& other)
		: face(nullptr, nullptr) {
		graphicsFactory = other.graphicsFactory;
		ftLibReference = other.ftLibReference;

		face = copyFace(other.face.get());
	}

	Font::Font(Font&& other) noexcept = default;

	Font& Font::operator=(const Font& other) {
		graphicsFactory = other.graphicsFactory;
		ftLibReference = other.ftLibReference;

		face = copyFace(other.face.get());

		return *this;
	}

	Font& Font::operator=(Font&& other) noexcept = default;

	Font::~Font() = default;

	void Font::setFontSize(uint32_t size) {
		FT_Set_Pixel_Sizes(face.get(), 0, static_cast<FT_UInt>(size));
	}

	Glyph Font::getChar(char ch) const {
		FT_Load_Char(face.get(), ch, FT_LOAD_RENDER);

		Glyph glyph{
			mth::vec2f{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
			mth::vec2f{ face->glyph->bitmap_left, face->glyph->bitmap_top },
			mth::vec2f{ face->glyph->advance.x >> 6, face->glyph->advance.y >> 6 }
		};

		unsigned char* faceBuffer = face->glyph->bitmap.buffer;

		if(faceBuffer == nullptr) {
			return glyph;
		}

		const uint8_t textureArraySize = 1;
		const TextureDescriptor descriptor{
			TextureStyle::Default,
			TextureUsage::Font,
			TextureFilter::Nearest,
			glyph.size,
			textureArraySize
		};

		auto texture = graphicsFactory->createTexture(descriptor, faceBuffer, 1);

		glyph.character = std::move(texture);
		return glyph;
	}

	Font::FacePtr Font::createFace(const std::string& filePath) {
		FT_Face face;
		if(FT_New_Face(ftLibReference.get(), filePath.c_str(), 0, &face) != FT_Err_Ok) {
			CLV_ASSERT(false, "Could not load font");
		}

		return makeUniqueFace(face);
	}

	Font::FacePtr Font::createFace(const unsigned char* bytes, const std::size_t size) {
		FT_Face face;
		if(FT_New_Memory_Face(ftLibReference.get(), bytes, size, 0, &face) != FT_Err_Ok) {
			CLV_ASSERT(false, "Could not load font");
		}

		return makeUniqueFace(face);
	}
}