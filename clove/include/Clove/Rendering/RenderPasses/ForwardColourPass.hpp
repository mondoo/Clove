#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace garlic::clove {
    class GhaFactory;
    class GhaPipelineObject;
    class GhaRenderPass;
}

namespace garlic::clove {
    class ForwardColourPass : public GeometryPass {
        //VARIABLES
    private:
        std::unique_ptr<GhaPipelineObject> pipeline{ nullptr };

        std::vector<Job> jobs{};

        //FUNCTIONS
    public:
        ForwardColourPass() = delete;
        ForwardColourPass(GhaFactory &ghaFactory, std::shared_ptr<GhaRenderPass> ghaRenderPass);//TEMP: Using an external render pass for now but these pass will need to create their own

        ForwardColourPass(ForwardColourPass const &other) = delete;
        ForwardColourPass(ForwardColourPass &&other) noexcept;

        ForwardColourPass &operator=(ForwardColourPass const &other) = delete;
        ForwardColourPass &operator=(ForwardColourPass &&other) noexcept;

        ~ForwardColourPass();

        void addJob(Job job) override;
        void flushJobs() override;

        void execute(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) override;
    };
}