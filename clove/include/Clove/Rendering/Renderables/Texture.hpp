#pragma once

#include <Clove/Maths/Vector.hpp>
#include <memory>
#include <functional>

namespace clove{
    class GhaImage;
    class GhaImageView;
}

namespace clove {
    class Texture {
        //VARIABLES
    private:
        std::unique_ptr<void, std::function<void(void *)>> buffer{ nullptr }; /**< Raw image data created from file. */
        std::unique_ptr<GhaImage> image{ nullptr };

        vec2ui dimensions{ 0, 0 };
        int32_t channels{ 0 };

        //FUNCTIONS
    public:
        Texture() = delete;
        Texture(std::unique_ptr<void, std::function<void(void *)>> buffer, vec2ui dimensions, int32_t channels);

        Texture(Texture const &other) = delete;
        Texture(Texture &&other) noexcept;

        Texture &operator=(Texture const &other) = delete;
        Texture &operator=(Texture &&other) noexcept;

        ~Texture();

        inline GhaImage *getImage() const;
    };
}

#include "Texture.inl"