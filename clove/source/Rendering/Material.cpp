#include "Clove/Rendering/Material.hpp"

#include "Clove/Application.hpp"
#include "Clove/Rendering/RenderingHelpers.hpp"

#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

namespace garlic::clove {
    std::weak_ptr<garlic::clove::GraphicsImage> Material::defaultImage{};

    Material::Material() {
        using namespace garlic::clove;

        if(defaultImage.use_count() == 0) {
            GraphicsFactory &factory = *Application::get().getGraphicsDevice()->getGraphicsFactory();

            vec2f constexpr imageDimensions{ 1.0f, 1.0f };
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

    void Material::setDiffuseTexture(std::shared_ptr<garlic::clove::GraphicsImage> image) {
        diffuseImage = std::move(image);
        diffuseView  = diffuseImage->createView(garlic::clove::GraphicsImageView::Descriptor{
            .type       = garlic::clove::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }

    void Material::setSpecularTexture(std::shared_ptr<garlic::clove::GraphicsImage> image) {
        specularImage = std::move(image);
        specularView  = specularImage->createView(garlic::clove::GraphicsImageView::Descriptor{
            .type       = garlic::clove::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }
}