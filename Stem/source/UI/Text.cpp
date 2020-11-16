#include "Stem/UI/Text.hpp"

#include "Stem/Rendering/ForwardRenderer3D.hpp"
#include "Stem/Application.hpp"

using namespace clv;
using namespace clv::gfx;

namespace garlic::inline stem {
    Text::Text(Font font)
        : font(std::move(font)) {
    }

    Text::Text(Text const &other) = default;

    Text::Text(Text &&other) = default;

    Text &Text::operator=(Text const &other) = default;

    Text &Text::operator=(Text &&other) = default;

    Text::~Text() = default;

    void Text::draw(clv::mth::vec2f const &drawSpace) {
        mth::vec2f const screenHalfSize{ drawSpace.x / 2.0f, drawSpace.y / 2.0f };

        mth::vec2f cursorPos{ position.x - screenHalfSize.x, -position.y + screenHalfSize.y };

        mth::mat4f const projection{ mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y) };

        for(size_t i = 0; i < getTextLength(); ++i) {
            Font::Glyph const &glyph{getBufferForCharAt(i)};

            //For spaces we just skip and proceed
            if(glyph.character != nullptr) {
                uint32_t const width{ glyph.size.x };
                uint32_t const height{ glyph.size.y };

                float const xpos{ cursorPos.x + glyph.bearing.x };
                float const ypos{ cursorPos.y - (height - glyph.bearing.y) };

                mth::mat4f model{ mth::translate(mth::mat4f{ 1.0f }, { xpos, ypos, 0.0f }) };
                model *= mth::scale(mth::mat4f{ 1.0f }, { width, height, 0.0f });

                Application::get().getRenderer()->submitText(glyph.characterView, projection * model);
            }

            cursorPos.x += glyph.advance.x;
        }
    }

    Font::Glyph const &Text::getBufferForCharAt(size_t index) {
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