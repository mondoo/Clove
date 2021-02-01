#pragma once

#include <Clove/Maths/Vector.hpp>
#include <memory>

namespace garlic::clove {
    class GhaFactory;
    class GhaImage;
    class GhaImageView;
}

namespace garlic::clove {
    /**
	 * @brief A Material contains values that will be passed to shaders to customise
	 * what an object looks like.
	 */
    class Material {
        friend class ForwardRenderer3D;

        //VARIABLES
    private:
        static std::weak_ptr<GhaImage> defaultImage;

        std::shared_ptr<GhaImage> diffuseImage;
        std::shared_ptr<GhaImage> specularImage;

        std::shared_ptr<GhaImageView> diffuseView;
        std::shared_ptr<GhaImageView> specularView;

        vec4f colour{ 1.0f };
        float shininess{ 32.0f };

        //FUNCTIONS
    public:
        Material();

        Material(Material const &other);
        Material(Material &&other) noexcept;

        Material &operator=(Material const &other);
        Material &operator=(Material &&other) noexcept;

        ~Material();

        void setDiffuseTexture(std::shared_ptr<garlic::clove::GhaImage> image);
        void setSpecularTexture(std::shared_ptr<garlic::clove::GhaImage> image);

        inline void setColour(vec4f colour);
        inline void setShininess(float shininess);
    };
}

#include "Material.inl"