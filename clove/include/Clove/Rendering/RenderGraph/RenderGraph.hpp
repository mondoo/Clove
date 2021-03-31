#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include <Clove/Maths/Vector.hpp>
#include <Clove/Graphics/GhaImage.hpp>

namespace garlic::clove{
    class GhaFactory;
    class GhaComputeQueue;
    class GhaGraphicsQueue;
    class GhaTransferQueue;
}

namespace garlic::clove {
    /**
     * @brief Tracks how resources are used to create a graph of how to render a frame.
     */
    class RenderGraph {
        //VARIABLES
    private:

        //FUNCTIONS
    public:
        RenderGraph();

        RenderGraph(RenderGraph const &other);
        RenderGraph(RenderGraph &&other) noexcept;

        RenderGraph &operator=(RenderGraph const &other);
        RenderGraph &operator=(RenderGraph &&other) noexcept;

        ~RenderGraph();

        /**
         * @brief Creates a RenderGraph tracked buffer.
         * @param bufferSize The size of the buffer in bytes.
         * @return 
         */
        RgBuffer createBuffer(size_t bufferSize);
        /**
         * @brief Create a RenderGraph tracked image.
         * @param imageType The type of the image (2D, 3D etc).
         * @param dimensions The dimensions of the image.
         * @return 
         */
        RgImage createImage(GhaImage::Type imageType, vec2ui dimensions);

        void addPass(/*TODO*/){}

        /**
         * @brief Executes the RenderGraph. Creating any objects required and submitting commands to the relevant queues.
         * @param factory Requires a factory to create any GHA objects when traversing the graph.
         * @param graphicsQueue The queue the graph will submit graphics work to.
         * @param computeQueue The queue the graph will submit compute work to.
         * @param transferQueue The queue the graph will submit transfer work to.
         */
        void execute(GhaFactory &factory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue);
    };
}