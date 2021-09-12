#pragma once

#include "Clove/Rendering/RenderGraph/RgResource.hpp"

#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaImageView.hpp>

namespace clove {
    class RgFrameCache;
}

namespace clove {
    class RgImage {
        friend class RgImageView;

        //VARIABLES
    private:
        GhaImage *ghaImage{ nullptr }; /**< The GHA image this object views. */
        GhaImage::Descriptor ghaImageDescriptor{};
        bool externalImage{ false }; /**< Will be true if this was registered with an external image. Preventing the descriptor being built. */

        //FUNCTIONS
    public:
        RgImage() = delete;
        RgImage(GhaImage::Type const imageType, GhaImage::Format const format, vec2ui const dimensions, uint32_t const arrayCount);
        RgImage(GhaImage *ghaImage);

        RgImage(RgImage const &other) = delete;
        RgImage(RgImage &&other) noexcept;

        RgImage &operator=(RgImage const &other) = delete;
        RgImage &operator=(RgImage &&other) noexcept;

        ~RgImage();
    };

    /**
     * @brief Tracks the usage and dependencies of an image view in the RenderGraph.
     * @details Can be used to later create a GhaImage when executing the graph.
     */
    class RgImageView : public RgResource {
        //VARIABLES
    private:
        RgImage *image{ nullptr };
        GhaImageView *ghaImageView{ nullptr };

        uint32_t arrayIndex{ 0 }; /**< The base index of the view. */
        uint32_t arrayCount{ 1 }; /**< how many elements in the array this views. */

        //FUNCTIONS
    public:
        RgImageView() = delete;
        RgImageView(RgImage *image, uint32_t const arrayIndex, uint32_t const arrayCount);

        RgImageView(RgImageView const &other) = delete;
        RgImageView(RgImageView &&other) noexcept;

        RgImageView &operator=(RgImageView const &other) = delete;
        RgImageView &operator=(RgImageView &&other) noexcept;

        ~RgImageView();

        /**
         * @brief Gets the underlying GHA image. Creates a new one if neccessary.
         * @param cache
         * @return 
         */
        GhaImage *getGhaImage(RgFrameCache &cache);
        /**
         * @brief Creates an image view for this object's internal image.
         * @param cache 
         * @param arrayIndex 
         * @param arrayCount 
         * @return 
         */
        GhaImageView *getGhaImageView(RgFrameCache &cache);

        inline GhaImage::Format getFormat() const;
        inline vec2ui const &getDimensions() const;
        inline bool isExternalImage() const;

        inline uint32_t getArrayIndex() const;
        inline uint32_t getArrayCount() const;

        void addImageUsage(GhaImage::UsageMode const usage);
    };
}

#include "RgImage.inl"