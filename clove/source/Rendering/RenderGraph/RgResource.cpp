#include "Clove/Rendering/RenderGraph/RgResource.hpp"

namespace garlic::clove {
    RgResource::RgResource(ResourceIdType id)
        : id{ id } {
    }

    RgResource::RgResource(RgResource &&other) noexcept = default;

    RgResource &RgResource::operator=(RgResource &&other) noexcept = default;

    RgResource::~RgResource() = default;
}