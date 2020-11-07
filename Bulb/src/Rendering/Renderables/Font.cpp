#include "Bulb/Rendering/Renderables/Font.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Root/Log/Log.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <Root/Definitions.hpp>

using namespace clv;
using namespace clv::gfx;

extern "C" const unsigned char roboto_black[];
extern "C" const size_t roboto_blackLength;

namespace blb::rnd {
    static Font::FacePtr makeUniqueFace(FT_Face face) {
        return Font::FacePtr(face, [](FT_Face face) { FT_Done_Face(face); });
    }

    static Font::FacePtr copyFace(FT_Face face) {
        if(FT_Reference_Face(face) != FT_Err_Ok) {
            GARLIC_ASSERT(false, "Could not reference face");
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
                GARLIC_ASSERT(false, "Could not load freetype");
            } else {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Constructed FreeType library");
            }

            auto const libraryDeleter = [](FT_Library lib) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "FreeType library has been deleted");
                FT_Done_FreeType(lib);
            };
            ftLibReference = FTLibSharedPtr(library, libraryDeleter);
            ftLib          = ftLibReference;
        } else {
            ftLibReference = ftLib.lock();
        }

        face = createFace({ reinterpret_cast<std::byte const *>(roboto_black), roboto_blackLength });
    }

    Font::Font(std::string const &filePath, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
        : Font(std::move(graphicsFactory)) {
        face = createFace(filePath);
    }

    Font::Font(std::span<std::byte const> bytes, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory)
        : Font(std::move(graphicsFactory)) {
        face = createFace(std::move(bytes));
    }

    Font::Font(Font const &other)
        : face(nullptr, nullptr) {
        graphicsFactory = other.graphicsFactory;
        ftLibReference  = other.ftLibReference;

        face = copyFace(other.face.get());
    }

    Font::Font(Font &&other) noexcept = default;

    Font &Font::operator=(Font const &other) {
        graphicsFactory = other.graphicsFactory;
        ftLibReference  = other.ftLibReference;

        face = copyFace(other.face.get());

        return *this;
    }

    Font &Font::operator=(Font &&other) noexcept = default;

    Font::~Font() = default;

    void Font::setFontSize(uint32_t size) {
        FT_Set_Pixel_Sizes(face.get(), 0, static_cast<FT_UInt>(size));
    }

    Glyph Font::getChar(char ch) const {
        using namespace clv::gfx;

        FT_Load_Char(face.get(), ch, FT_LOAD_RENDER);

        Glyph glyph{
            .size    = { face->glyph->bitmap.width, face->glyph->bitmap.rows },
            .bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top },
            .advance = { face->glyph->advance.x >> 6, face->glyph->advance.y >> 6 }
        };

        unsigned char *faceBuffer = face->glyph->bitmap.buffer;

        if(faceBuffer == nullptr) {
            return glyph;
        }

        size_t constexpr bytesPerPixel{ 4 };
        size_t const sizeBytes{ static_cast<size_t>(glyph.size.x) * static_cast<size_t>(glyph.size.y) * bytesPerPixel };

        auto transferQueue = graphicsFactory->createTransferQueue({ QueueFlags::Transient });

        std::shared_ptr<TransferCommandBuffer> transferCommandBuffer = transferQueue->allocateCommandBuffer();

        auto transferBuffer = graphicsFactory->createBuffer(GraphicsBuffer::Descriptor{
            .size        = sizeBytes,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferSource,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        });
        transferBuffer->write(faceBuffer, 0, sizeBytes);

        glyph.character = graphicsFactory->createImage(GraphicsImage::Descriptor{
            .type        = GraphicsImage::Type::_2D,
            .usageFlags  = GraphicsImage::UsageMode::Sampled | GraphicsImage::UsageMode::TransferDestination,
            .dimensions  = glyph.size,
            .format      = GraphicsImage::Format::R8G8B8A8_SRGB,
            .sharingMode = SharingMode::Concurrent,
            .memoryType  = MemoryType::SystemMemory,
        });

        transferCommandBuffer->copyBufferToImage(*transferBuffer, 0, *glyph.character, GraphicsImage::Layout::Undefined, { 0, 0, 0 }, { glyph.size.x, glyph.size.y, 1 });

        return glyph;
    }

    Font::FacePtr Font::createFace(std::string const &filePath) {
        FT_Face face;
        if(FT_New_Face(ftLibReference.get(), filePath.c_str(), 0, &face) != FT_Err_Ok) {
            GARLIC_ASSERT(false, "Could not load font");
        }

        return makeUniqueFace(face);
    }

    Font::FacePtr Font::createFace(std::span<std::byte const> bytes) {
        FT_Face face;
        if(FT_New_Memory_Face(ftLibReference.get(), reinterpret_cast<unsigned char const *>(bytes.data()), bytes.size_bytes(), 0, &face) != FT_Err_Ok) {
            GARLIC_ASSERT(false, "Could not load font");
        }

        return makeUniqueFace(face);
    }
}