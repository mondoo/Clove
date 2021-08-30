#pragma once

#include "Clove/Rendering/RenderGraph/RgResource.hpp"

#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaImageView.hpp>
#include <memory>

namespace clove {
    class RgFrameCache;
}

namespace clove {
    /**
     * @brief Tracks the usage and dependencies of an image in the RenderGraph.
     * @details Can be used to later create a GhaImage when executing the graph.
     */
    class RgImage : public RgResource {
        //VARIABLES
    private:
        GhaImage *ghaImage{ nullptr }; /**< The GHA image this object views. */
        GhaImageView *ghaImageView{ nullptr }; /**< The GHA image view this object represents. */
        GhaImage::Descriptor ghaImageDescriptor{};
        bool externalImage{ false }; /**< Will be true if this was registered with an external image. Preventing the descriptor being built. */

        //FUNCTIONS
    public:
        RgImage() = delete;
        RgImage(RgResourceIdType id, GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions, uint32_t arrayCount);
        RgImage(RgResourceIdType id, GhaImage *ghaImage);

        RgImage(RgImage const &other) = delete;
        RgImage(RgImage &&other) noexcept;

        RgImage &operator=(RgImage const &other) = delete;
        RgImage &operator=(RgImage &&other) noexcept;

        ~RgImage();

        /**
         * @brief Get the GhaImageView for this object. Will create any resources required.
         * @param ghaFactory 
         * @return 
         */
        GhaImageView *getGhaImageView(RgFrameCache &cache, uint32_t const arrayIndex, uint32_t const arrayCount);

        inline bool isExternalImage() const;

        void addImageUsage(GhaImage::UsageMode usage);
    };
}

#include "RgImage.inl"