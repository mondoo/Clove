#pragma once

#include "Bulb/Rendering/Renderables/Font.hpp"
#include "Bulb/UI/DrawableElement.hpp"

namespace clv::gfx {
    class GrpahicsFactory;
}

namespace blb::ui {
    class Text : public DrawableElement {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        rnd::Font font;
        std::string text;

        bool isBufferDirty{ false };
        std::vector<rnd::Glyph> characters;

        clv::mth::vec2f position{ 0.0f, 0.0f };
        float rotation{ 0.0f };
        clv::mth::vec2f scale{ 1.0f, 1.0f };

        //FUNCTIONS
    public:
        Text() = delete;
        Text(rnd::Font font);

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

        void draw(rnd::ForwardRenderer3D &renderer, clv::mth::vec2f const &drawSpace) override;

        inline void setText(std::string text);
        inline void setFontSize(uint32_t const size);

        inline std::size_t getTextLength() const;
        rnd::Glyph const &getBufferForCharAt(size_t index);

    private:
        void buildGlyphs();
    };
}

#include "Text.inl"