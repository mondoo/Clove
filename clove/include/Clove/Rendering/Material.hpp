#pragma once

#include "Clove/FileSystem/AssetPtr.hpp"
#include "Clove/Rendering/Renderables/Texture.hpp"

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
        //VARIABLES
    private:
        //Default images and views - fall backs for if a texture hasn't been provided.
        static std::shared_ptr<GhaImage> defaultImage;
        static std::shared_ptr<GhaImageView> defaultView;

        AssetPtr<Texture> diffuseTexture;
        AssetPtr<Texture> specularTexture;

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

        inline void setDiffuseTexture(AssetPtr<Texture> texture);
        std::shared_ptr<GhaImageView> getDiffuseView() const;

        inline void setSpecularTexture(AssetPtr<Texture> texture);
        std::shared_ptr<GhaImageView> getSpecularView() const;

        inline void setColour(vec4f colour);
        inline vec4f const &getColour() const;

        inline void setShininess(float shininess);
        inline float getShininess() const;
    };
}

#include "Material.inl"