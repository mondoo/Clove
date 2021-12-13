#include "Clove/Rendering/Material.hpp"

#include "Clove/Application.hpp"
#include "Clove/Rendering/RenderingHelpers.hpp"

#include <Clove/Graphics/GhaDevice.hpp>
#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaImageView.hpp>

namespace clove {
    std::weak_ptr<GhaImage> Material::defaultImage{};

    Material::Material() {
        if(defaultImage.use_count() == 0) {
            GhaFactory &factory{ *Application::get().getGraphicsDevice()->getGraphicsFactory() };

            vec2f constexpr imageDimensions{ 1.0f, 1.0f };
            uint32_t constexpr bytesPerTexel{ 4 };
            uint32_t constexpr white{ 0xffffffff };

            GhaImage::Descriptor const imageDescriptor{
                .type        = GhaImage::Type::_2D,
                .usageFlags  = GhaImage::UsageMode::TransferDestination | GhaImage::UsageMode::Sampled,
                .dimensions  = imageDimensions,
                .format      = GhaImage::Format::R8G8B8A8_SRGB,
                .sharingMode = SharingMode::Exclusive,
            };

            std::shared_ptr<GhaImage> image{ createImageWithData(factory, imageDescriptor, &white, bytesPerTexel) };

            defaultImage      = image;
            localDefaultImage = image;
        } else {
            localDefaultImage = defaultImage.lock();
        }
    }

    Material::Material(Material const &other) = default;

    Material::Material(Material &&other) noexcept = default;

    Material &Material::operator=(Material const &other) = default;

    Material &Material::operator=(Material &&other) noexcept = default;

    Material::~Material() = default;

    GhaImage *Material::getDiffuseImage() const {
        if(diffuseTexture.isValid()) {
            return diffuseTexture->getImage();
        } else {
            return localDefaultImage.get();
        }
    }

    GhaImage *Material::getSpecularImage() const {
        if(specularTexture.isValid()) {
            return specularTexture->getImage();
        } else {
            return localDefaultImage.get();
        }
    }
}