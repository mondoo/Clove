#pragma once

#include <Clove/Maths/Vector.hpp>
#include <memory>

namespace garlic::clove {
    class GraphicsFactory;
    class GraphicsImage;
    class GraphicsImageView;
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
        static std::weak_ptr<GraphicsImage> defaultImage;

        std::shared_ptr<GraphicsImage> diffuseImage;
        std::shared_ptr<GraphicsImage> specularImage;

        std::shared_ptr<GraphicsImageView> diffuseView;
        std::shared_ptr<GraphicsImageView> specularView;

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

        void setDiffuseTexture(std::shared_ptr<garlic::clove::GraphicsImage> image);
        void setSpecularTexture(std::shared_ptr<garlic::clove::GraphicsImage> image);

        inline void setColour(vec4f colour);
        inline void setShininess(float shininess);
    };
}

#include "Material.inl"