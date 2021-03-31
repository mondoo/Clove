#include "Clove/Rendering/RenderGraph/RgNode.hpp"

namespace garlic::clove {
    RgNode::RgNode() = default;

    RgNode::RgNode(RgNode &&other) noexcept = default;

    RgNode &RgNode::operator=(RgNode &&other) noexcept = default;

    RgNode::~RgNode() = default;

    void RgNode::addDependecy(RgNode *dependency) {
        dependencies.push_back(dependency);
    }
}