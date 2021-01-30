#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace garlic::clove {
    class GhaFactory;
    class GhaPipelineObject;
    class GhaRenderPass;
}

namespace garlic::clove {
    //TODO + Own file
    /* class AnimatedColourPass : public GeometryPass{
    }; */

    //TODO: StaticColourPass
    class ForwardColourPass : public GeometryPass {
        //VARIABLES
    private:
        GhaFactory *ghaFactory{ nullptr };

        std::unique_ptr<GhaPipelineObject> pipeline;

        std::vector<Job> jobs;
        //std::unique_ptr<GhaPipelineObject> animatedMeshPipeline;

        //FUNCTIONS
    public:
        //TODO: Ctors
        ForwardColourPass(GhaFactory *ghaFactory, std::shared_ptr<GhaRenderPass> ghaRenderPass);//TEMP: Using an external render pass for now but these pass will need to create their own
        ~ForwardColourPass();

        void addJob(Job job) override;

        void flushJobs(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) override;
    };
}