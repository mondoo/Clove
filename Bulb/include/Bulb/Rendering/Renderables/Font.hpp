#pragma once

#include <span>

//Forward dec (see freetype.h)
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_ *FT_Face;

namespace clv::gfx {
    class GraphicsFactory;
    class GraphicsImage;
}

namespace blb::rnd {
    struct Glyph {
        clv::mth::vec2ui size{};
        clv::mth::vec2i bearing{};
        clv::mth::vec2i advance{};
        std::shared_ptr<clv::gfx::GraphicsImage> character;
    };
}

namespace blb::rnd {
    class Font {
        //TYPES
    public:
        using FTLibWeakPtr   = std::weak_ptr<std::remove_pointer_t<FT_Library>>;
        using FTLibSharedPtr = std::shared_ptr<std::remove_pointer_t<FT_Library>>;

        using FacePtr = std::unique_ptr<std::remove_pointer_t<FT_Face>, void (*)(FT_Face)>;

        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        static FTLibWeakPtr ftLib;
        FTLibSharedPtr ftLibReference;

        FacePtr face;

        //FUNCTIONS
    public:
        Font() = delete;
        Font(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
        Font(std::string const &filePath, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
        Font(std::span<std::byte const> bytes, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);

        Font(Font const &other);
        Font(Font &&other) noexcept;

        Font &operator=(Font const &other);
        Font &operator=(Font &&other) noexcept;

        ~Font();

        void setFontSize(uint32_t size);
        Glyph getChar(char ch) const;

    private:
        FacePtr createFace(std::string const &filePath);
        FacePtr createFace(std::span<std::byte const> bytes);
    };
}