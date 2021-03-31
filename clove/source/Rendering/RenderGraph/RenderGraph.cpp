#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

namespace garlic::clove {
    RenderGraph::RenderGraph() = default;

    RenderGraph::RenderGraph(RenderGraph const &other) = default;

    RenderGraph::RenderGraph(RenderGraph &&other) noexcept = default;

    RenderGraph &RenderGraph::operator=(RenderGraph const &other) = default;

    RenderGraph &RenderGraph::operator=(RenderGraph &&other) noexcept = default;

    RenderGraph::~RenderGraph() = default;

    RgBuffer RenderGraph::createBuffer(size_t bufferSize) {
        return RgBuffer{ bufferSize };
    }

    RgImage RenderGraph::createImage(GhaImage::Type imageType, vec2ui dimensions) {
        return RgImage{ imageType, std::move(dimensions) };
    }

    void RenderGraph::execute(GhaFactory &factory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue) {
        //TODO:
    }
}