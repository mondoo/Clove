#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

namespace clove {
    GeometryPass::GeometryPass() = default;

    GeometryPass::GeometryPass(GeometryPass const &other) = default;

    GeometryPass::GeometryPass(GeometryPass &&other) noexcept = default;

    GeometryPass &GeometryPass::operator=(GeometryPass const &other) = default;

    GeometryPass &GeometryPass::operator=(GeometryPass &&other) noexcept = default;

    GeometryPass::~GeometryPass() = default;
}