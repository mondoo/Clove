#include "Clove/Rendering/RenderGraph/RgResource.hpp"

namespace clove {
    RgResource::RgResource() = default;

    RgResource::RgResource(RgResource &&other) noexcept = default;

    RgResource &RgResource::operator=(RgResource &&other) noexcept = default;

    RgResource::~RgResource() = default;
}