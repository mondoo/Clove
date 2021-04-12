#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgComputePipelineState.hpp"
#include "Clove/Rendering/RenderGraph/RgGraphicsPipelineState.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include <Clove/Graphics/GhaGraphicsQueue.hpp>
#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Maths/Vector.hpp>
#include <functional>

namespace garlic::clove {
    class GhaFactory;
    class GhaComputeQueue;
    class GhaGraphicsQueue;
    class GhaTransferQueue;
    class GhaGraphicsCommandBuffer;
    class GhaComputeCommandBuffer;
}

namespace garlic::clove {
    /**
     * @brief Tracks how resources are used to create a graph of how to render a frame.
     */
    class RenderGraph {
        //TYPES
    public:
        struct BufferBinding {
            uint32_t slot{};
            RgBuffer buffer{};
        };

        struct ImageBindng {
            uint32_t slot{};
            GhaSampler::Descriptor samplerState{};//TODO: RG version?
            RgImage image{};
        };

        /**
         * @brief Represents a single draw indexed call.
         */
        struct GraphicsSubmission {
            RgBuffer vertexBuffer{};
            RgBuffer indexBuffer{};

            //NOTE: Will be used to create descriptor sets
            std::vector<BufferBinding> shaderUbos{};
            std::vector<ImageBindng> shaderCombinedImageSamplers{};
        };

        /**
         * @brief Represents a single dispatch call.
         */
        struct ComputeSubmission {
            vec3ui dispatchSize{ 1, 1, 1 };

            //NOTE: Will be used to create descriptor sets
            std::vector<BufferBinding> shaderStorageBuffers{};
            std::vector<ImageBindng> shaderStorageImages{};//TODO: Sampler not required on compute passes
        };

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

        //TODO: Maybe a version that just takes a vertex / index buffer?
        void addGraphicsPass(RgGraphicsPipelineState pipelineState, std::vector<GraphicsSubmission> pass) {
            //TODO

            //The idea is that it tracks the pipeline (incase of being used in multiple passes)
            //Then will bind the pipeline and execute the pass

            //I think it's expected (for now) to bind descriptor sets with in the provided pass
        }

        //TODO: Maybe a version that just adds a dispatch call
        void addComputePass(RgComputePipelineState pipelineState, std::vector<ComputeSubmission> pass) {
            //TODO
        }

        /**
         * @brief Executes the RenderGraph. Creating any objects required and submitting commands to the relevant queues.
         * @param factory Requires a factory to create any GHA objects when traversing the graph.
         * @param graphicsQueue The queue the graph will submit graphics work to.
         * @param computeQueue The queue the graph will submit compute work to.
         * @param transferQueue The queue the graph will submit transfer work to.
         * @return Returns the GraphicsSubmitInfo used to render the final result of the graph.
         */
        GraphicsSubmitInfo execute(GhaFactory &factory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue);
    };
}