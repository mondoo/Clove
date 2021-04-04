#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

namespace garlic::clove {
    RenderGraph::RenderGraph() = default;

    RenderGraph::RenderGraph(RenderGraph const &other) = default;

    RenderGraph::RenderGraph(RenderGraph &&other) noexcept = default;

    RenderGraph &RenderGraph::operator=(RenderGraph const &other) = default;

    RenderGraph &RenderGraph::operator=(RenderGraph &&other) noexcept = default;

    RenderGraph::~RenderGraph() = default;

    //Return a submit info?
    void RenderGraph::execute(GhaFactory &factory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue) {
        //TODO

        /*
        Do I want to submit everything here or just return a bunch of render passes?

        have a RgPass (RgGraphicsPass / RgComputePass)
        Passes will take the buffers / textures
        Render graph orders these based on dependencies
        renderer traverses the graph and provides the actual textures etc.
        At the moment I think this should submit everything to the queue. The only problem is sending everything to the render target

        Have 2 things:
            - resources (buffers / images)
            - passes (execute shaders. consume and produce resources)
        */
    }
}