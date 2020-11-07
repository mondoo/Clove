#pragma once

#include "Bulb/UI/DrawableElement.hpp"

namespace clv::gfx {
    class GraphicsFactory;
    class GraphicsImage;
}

namespace blb::ui {
    class Image : public DrawableElement {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsImage> image;

        clv::mth::vec2f position{ 0.0f, 0.0f };
        float rotation{ 0.0f };
        clv::mth::vec2f size{ 1.0f, 1.0f };

        //FUNCTIONS
    public:
        Image() = delete;
        Image(clv::gfx::GraphicsFactory& factory);
        Image(std::shared_ptr<clv::gfx::GraphicsImage> graphicsImage);

        Image(Image const &other);
        Image(Image &&other) noexcept;

        Image &operator=(Image const &other);
        Image &operator=(Image &&other) noexcept;

        ~Image();

        inline void setPosition(clv::mth::vec2f position);
        inline void setRotation(float rotation);
        inline void setSize(clv::mth::vec2f size);

        inline clv::mth::vec2f const &getPosition() const;
        inline float getRotation() const;
        inline clv::mth::vec2f const &getSize() const;

        void draw(rnd::ForwardRenderer3D &renderer, clv::mth::vec2f const &drawSpace) override;
    };
}

#include "Image.inl"