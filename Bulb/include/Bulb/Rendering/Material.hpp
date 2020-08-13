#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class GraphicsFactory;
    class GraphicsImage;
    class GraphicsImageView;
}

namespace blb::rnd {
    /**
	 * @brief A Material contains values that will be passed to shaders to customise
	 * what an object looks like.
	 */
    class Material {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::GraphicsImage> defaultImage;

        std::shared_ptr<clv::gfx::GraphicsImage> diffuseImage;
        std::shared_ptr<clv::gfx::GraphicsImage> specularImage;

        std::shared_ptr<clv::gfx::GraphicsImageView> diffuseView;
        std::shared_ptr<clv::gfx::GraphicsImageView> specularView;

        clv::mth::vec4f colour{ 1.0f };
        float shininess{ 32.0f };

        //FUNCTIONS
    public:
        Material() = delete;
        Material(const std::shared_ptr<clv::gfx::GraphicsFactory>& graphicsFactory);

        Material(const Material& other);
        Material(Material&& other) noexcept;

        Material& operator=(const Material& other);
        Material& operator=(Material&& other) noexcept;

        ~Material();

        void setDiffuseTexture(std::shared_ptr<clv::gfx::GraphicsImage> image);
        void setSpecularTexture(std::shared_ptr<clv::gfx::GraphicsImage> image);

        void setColour(clv::mth::vec4f colour);
        void setShininess(float shininess);
    };
}