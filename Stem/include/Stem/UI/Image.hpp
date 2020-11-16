#pragma once

#include "Stem/UI/DrawableElement.hpp"

namespace clv::gfx {
    class GraphicsFactory;
    class GraphicsImage;
    class GraphicsImageView;
}

namespace garlic::inline stem {
    class Image : public DrawableElement {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsImage> image;
        std::shared_ptr<clv::gfx::GraphicsImageView> imageView;

        clv::mth::vec2f position{ 0.0f, 0.0f };
        float rotation{ 0.0f };
        clv::mth::vec2f size{ 1.0f, 1.0f };

        //FUNCTIONS
    public:
        Image();
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

        void draw(clv::mth::vec2f const &drawSpace) override;
    };
}

#include "Image.inl"