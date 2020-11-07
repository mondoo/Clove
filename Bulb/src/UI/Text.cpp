#include "Bulb/UI/Text.hpp"

#include "Bulb/Rendering/ForwardRenderer3D.hpp"

using namespace clv;
using namespace clv::gfx;

namespace blb::ui {
    Text::Text(rnd::Font font)
        : font(std::move(font)) {
    }

    Text::Text(Text const &other) = default;

    Text::Text(Text &&other) = default;

    Text &Text::operator=(Text const &other) = default;

    Text &Text::operator=(Text &&other) = default;

    Text::~Text() = default;

    void Text::draw(rnd::ForwardRenderer3D &renderer, clv::mth::vec2f const &drawSpace) {
        GraphicsImageView::Descriptor constexpr viewDescriptor{
            .type       = GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        };

        mth::vec2f const screenHalfSize{ static_cast<float>(drawSpace.x) / 2.0f, static_cast<float>(drawSpace.y) / 2.0f };

        mth::vec2f cursorPos{ position.x - screenHalfSize.x, -position.y + screenHalfSize.y };

        mth::mat4f const projection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);

        for(size_t i = 0; i < getTextLength(); ++i) {
            rnd::Glyph const &glyph = getBufferForCharAt(i);

            //For spaces we just skip and proceed
            if(glyph.character != nullptr) {
                float const width  = glyph.size.x;
                float const height = glyph.size.y;

                float const xpos = cursorPos.x + glyph.bearing.x;
                float const ypos = cursorPos.y - (height - glyph.bearing.y);

                mth::mat4f model = mth::mat4f(1.0f);
                model            = mth::translate(mth::mat4f(1.0f), { xpos, ypos, 0.0f });
                model *= mth::scale(mth::mat4f(1.0f), { width, height, 0.0f });

                renderer.submitText(glyph.character->createView(viewDescriptor), model * projection);
            }

            cursorPos.x += glyph.advance.x;
        }
    }

    rnd::Glyph const &Text::getBufferForCharAt(size_t index) {
        if(isBufferDirty) {
            buildGlyphs();
        }
        return characters[index];
    }

    void Text::buildGlyphs() {
        characters.clear();
        for(size_t i = 0; i < text.length(); ++i) {
            characters.emplace_back(font.getChar(text[i]));
        }
        isBufferDirty = false;
    }
}