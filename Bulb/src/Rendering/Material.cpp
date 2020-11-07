#include "Bulb/Rendering/Material.hpp"

#include "Bulb/Rendering/RenderingHelpers.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

namespace blb::rnd {
    std::weak_ptr<clv::gfx::GraphicsImage> Material::defaultImage{};

    Material::Material(clv::gfx::GraphicsFactory &factory) {
        using namespace clv::gfx;

        if(defaultImage.use_count() == 0) {
            clv::mth::vec2f constexpr imageDimensions{ 1.0f, 1.0f };
            uint32_t constexpr bytesPerTexel{ 4 };
            uint32_t constexpr white{ 0xffffffff };

            GraphicsImage::Descriptor const imageDescriptor{
                .type        = GraphicsImage::Type::_2D,
                .usageFlags  = GraphicsImage::UsageMode::TransferDestination | GraphicsImage::UsageMode::Sampled,
                .dimensions  = imageDimensions,
                .format      = GraphicsImage::Format::R8G8B8A8_SRGB,
                .sharingMode = SharingMode::Exclusive,
            };

            std::shared_ptr<GraphicsImage> image = createImageWithData(factory, std::move(imageDescriptor), &white, bytesPerTexel);

            defaultImage = image;

            diffuseImage  = image;
            specularImage = image;
        } else {
            diffuseImage  = defaultImage.lock();
            specularImage = defaultImage.lock();
        }

        GraphicsImageView::Descriptor const viewDescriptor{
            .type       = GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        };

        diffuseView  = diffuseImage->createView(viewDescriptor);
        specularView = specularImage->createView(std::move(viewDescriptor));
    }

    Material::Material(Material const &other) = default;

    Material::Material(Material &&other) noexcept = default;

    Material &Material::operator=(Material const &other) = default;

    Material &Material::operator=(Material &&other) noexcept = default;

    Material::~Material() = default;

    void Material::setDiffuseTexture(std::shared_ptr<clv::gfx::GraphicsImage> image) {
        diffuseImage = std::move(image);
        diffuseView  = diffuseImage->createView(clv::gfx::GraphicsImageView::Descriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }

    void Material::setSpecularTexture(std::shared_ptr<clv::gfx::GraphicsImage> image) {
        specularImage = std::move(image);
        specularView  = specularImage->createView(clv::gfx::GraphicsImageView::Descriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }
}