#include "Stem/UI/Image.hpp"

#include "Stem/Rendering/ForwardRenderer3D.hpp"
#include "Stem/Rendering/RenderingHelpers.hpp"

#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

using namespace clv;
using namespace clv::gfx;

namespace garlic::inline stem {
    Image::Image(GraphicsFactory &factory) {
        clv::mth::vec2ui constexpr imageDimensions{ 1, 1 };
        uint32_t constexpr bytesPerTexel{ 4 };
        uint32_t constexpr white{ 0xffffffff };

        GraphicsImage::Descriptor constexpr imageDescriptor{
            .type        = GraphicsImage::Type::_2D,
            .usageFlags  = GraphicsImage::UsageMode::TransferDestination | GraphicsImage::UsageMode::Sampled,
            .dimensions  = imageDimensions,
            .format      = GraphicsImage::Format::R8G8B8A8_SRGB,
            .sharingMode = SharingMode::Exclusive,
        };

        image     = createImageWithData(factory, std::move(imageDescriptor), &white, bytesPerTexel);
        imageView = image->createView(GraphicsImageView::Descriptor{
            .type       = GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });
    }

    Image::Image(std::shared_ptr<clv::gfx::GraphicsImage> graphicsImage)
        : image(std::move(image)) {
    }

    Image::Image(Image const &other) = default;

    Image::Image(Image &&other) noexcept = default;

    Image &Image::operator=(Image const &other) = default;

    Image &Image::operator=(Image &&other) noexcept = default;

    Image::~Image() = default;

    void Image::draw(ForwardRenderer3D &renderer, clv::mth::vec2f const &drawSpace) {
        mth::vec2f const screenHalfSize{ drawSpace / 2.0f };

        //Move the position to origin at the top left
        mth::vec2f const pos{ position.x - screenHalfSize.x, -position.y + screenHalfSize.y };

        mth::mat4f const translation{ mth::translate(mth::mat4f{ 1.0f }, { pos, 0.0f }) };
        mth::mat4f const rotation{ mth::rotate(mth::mat4f{ 1.0f }, this->rotation, { 0.0f, 0.0f, 1.0f }) };
        mth::mat4f const scale{ mth::scale(mth::mat4f{ 1.0f }, mth::vec3f{ size, 0.0f }) };

        mth::mat4f const model{ translation * rotation * scale };
        mth::mat4f const projection{ mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y) };

        renderer.submitWidget(imageView, projection * model);
    }
}