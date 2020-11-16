#pragma once

#include "Stem/Rendering/Renderables/Font.hpp"
#include "Stem/UI/DrawableElement.hpp"

namespace clv::gfx {
    class GrpahicsFactory;
}

namespace garlic::inline stem {
    class Text : public DrawableElement {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        Font font;
        std::string text;

        bool isBufferDirty{ false };
        std::vector<Font::Glyph> characters;

        clv::mth::vec2f position{ 0.0f, 0.0f };
        float rotation{ 0.0f };
        clv::mth::vec2f scale{ 1.0f, 1.0f };

        //FUNCTIONS
    public:
        Text() = delete;
        Text(Font font);

        Text(Text const &other);
        Text(Text &&other);

        Text &operator=(Text const &other);
        Text &operator=(Text &&other);

        ~Text();

        inline void setPosition(clv::mth::vec2f position);
        inline void setRotation(float rotation);
        inline void setScale(clv::mth::vec2f scale);

        inline clv::mth::vec2f const &getPosition() const;
        inline float getRotation() const;
        inline clv::mth::vec2f const &getScale() const;

        void draw(clv::mth::vec2f const &drawSpace) override;

        inline void setText(std::string text);
        inline void setFontSize(uint32_t const size);

        inline std::size_t getTextLength() const;
        Font::Glyph const &getBufferForCharAt(size_t index);

    private:
        void buildGlyphs();
    };
}

#include "Text.inl"