#pragma once

namespace clv::gfx {
    class GraphicsFactory;
    class GraphicsImage;
    class GraphicsImageView;
}

namespace garlic::inline stem {
    /**
	 * @brief A Material contains values that will be passed to shaders to customise
	 * what an object looks like.
	 */
    class Material {
        friend class ForwardRenderer3D;

        //VARIABLES
    private:
        static std::weak_ptr<clv::gfx::GraphicsImage> defaultImage;

        std::shared_ptr<clv::gfx::GraphicsImage> diffuseImage;
        std::shared_ptr<clv::gfx::GraphicsImage> specularImage;

        std::shared_ptr<clv::gfx::GraphicsImageView> diffuseView;
        std::shared_ptr<clv::gfx::GraphicsImageView> specularView;

        clv::mth::vec4f colour{ 1.0f };
        float shininess{ 32.0f };

        //FUNCTIONS
    public:
        Material() = delete;
        Material(clv::gfx::GraphicsFactory &factory);

        Material(Material const &other);
        Material(Material &&other) noexcept;

        Material &operator=(Material const &other);
        Material &operator=(Material &&other) noexcept;

        ~Material();

        void setDiffuseTexture(std::shared_ptr<clv::gfx::GraphicsImage> image);
        void setSpecularTexture(std::shared_ptr<clv::gfx::GraphicsImage> image);

        inline void setColour(clv::mth::vec4f colour);
        inline void setShininess(float shininess);
    };
}

#include "Material.inl"